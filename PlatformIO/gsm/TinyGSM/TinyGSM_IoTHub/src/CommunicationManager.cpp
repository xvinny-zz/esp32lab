#include "CommunicationManager.h"

//////////////////////////////////
//           VARIÁVEIS          //
//////////////////////////////////
#if defined(OPERATOR_TIM)
const char apn[] = "timbrasil.br";
const char user[] = "tim";
const char pass[] = "tim";
#elif defined(OPERATOR_OI)
const char apn[] = "gprs.oi.com.br";
const char user[] = "oi";
const char pass[] = "oi";
#elif defined(OPERATOR_VIVO)
const char apn[] = "zap.vivo.com.br";
const char user[] = "vivo";
const char pass[] = "vivo";
#else
const char apn[] = "timbrasil.br";
const char user[] = "tim";
const char pass[] = "tim";
#endif

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);    
#else
static TinyGsm _modem(SerialAT);
static TinyGsmClient _client(_modem);
static PubSubClient _mqtt(_client);
#endif

static volatile bool s_isMonitorRunning = false;
static TaskHandle_t * s_monitorTaskHandle;
static TaskHandle_t * s_mqttTaskHandle;
static SemaphoreHandle_t s_monitorMutex;
static ConnectionProperties s_connectionProperties;
static unsigned long s_lastReconnectAttempt;
static MQTTSettings_t s_mqttSettings;

const char *topicLed = "GsmClient/led";
const char *topicInit = "GsmClient/init";
const char *topicLedStatus = "GsmClient/ledStatus";

void monitorConnectivity( void *pvParameters ) 
{
    ESP_LOGE("[CM]", "Iniciando o monitoramento de conectividade");

    while (1) 
    {
        if ( s_connectionProperties.IsModemStarted == false )
        {
            ESP_LOGI("[CM]", "Restartando o modem");
            xSemaphoreTake( s_monitorMutex, portMAX_DELAY );
                s_connectionProperties.IsModemStarted = _modem.restart();
            xSemaphoreGive( s_monitorMutex );

            ESP_LOGI("[CM]", "Modem restartado.");
            if ( s_connectionProperties.IsModemStarted == false )
            {
                ESP_LOGE("[CM]", "Nao foi possivel reiniciar o modem GSM... Aguardando 1s para tentar novamente...");
                delay(1000);
            }
        }
        else
        {
            // Se ja esta startado, verifica a conectividade com a rede continuamente...
            if (s_connectionProperties.IsNetworkConnected == false)
            {
                ESP_LOGI("[CM]", "Nao esta conectado a rede GSM. Aguardando rede por ate 60s...");
                xSemaphoreTake( s_monitorMutex, portMAX_DELAY );
                    s_connectionProperties.IsNetworkConnected = _modem.waitForNetwork();
                xSemaphoreGive( s_monitorMutex );
                
                if ( s_connectionProperties.IsNetworkConnected == false )
                {
                    ESP_LOGI("[CM]", "Continua nao conectado a rede GSM. Tentaremos novamente apos alguns instantes...");
                }
            }
            else
            {
                // Se ja esta conectado na rede, verifica a conectividade GPRS...
                ESP_LOGI("[CM]", "Conectado a rede GSM. Verificando a conectividade GPRS...");
                xSemaphoreTake( s_monitorMutex, portMAX_DELAY );
                    s_connectionProperties.IsGPRSConnected = _modem.isGprsConnected();
                    s_connectionProperties.IsAnyConnected = s_connectionProperties.IsGPRSConnected;
                xSemaphoreGive( s_monitorMutex );
                
                if ( s_connectionProperties.IsGPRSConnected == false )
                {
                    ESP_LOGI("[CM]", "Tentando conectar na GPRS...");
                    xSemaphoreTake( s_monitorMutex, portMAX_DELAY );
                        s_connectionProperties.IsGPRSConnected = _modem.gprsConnect( apn, user, pass );
                    xSemaphoreGive( s_monitorMutex );
                    
                    if ( s_connectionProperties.IsGPRSConnected == false )
                    {
                        ESP_LOGE("[CM]", "Falha ao tentar conectar via GPRS. Tentando novamente em alguns instantes...");
                        xSemaphoreTake( s_monitorMutex, portMAX_DELAY );
                            s_connectionProperties.IsAnyConnected = false;
                            s_connectionProperties.IsGPRSConnected = false;
                            s_connectionProperties.IsNetworkConnected = false; 
                            s_connectionProperties.IsModemStarted = false;
                        xSemaphoreGive( s_monitorMutex );
                    }
                    else
                    {
                        ESP_LOGI("[CM]", "Conectado via GPRS com sucesso...");
                        xSemaphoreTake( s_monitorMutex, portMAX_DELAY );
                            s_connectionProperties.IsAnyConnected = true;
                            s_connectionProperties.IsGPRSConnected = true;
                            s_connectionProperties.IsNetworkConnected = true; 
                            s_connectionProperties.IsModemStarted = true;
                        xSemaphoreGive( s_monitorMutex );
                    }
                }
                else
                {  
                    xSemaphoreTake( s_monitorMutex, portMAX_DELAY );
                        s_connectionProperties.Operator = _modem.getOperator();
                        s_connectionProperties.LocalIP = _modem.localIP();
                        s_connectionProperties.SignalQuality = _modem.getSignalQuality();
                    xSemaphoreGive( s_monitorMutex );

                    ESP_LOGI("[CM]", "Core: [%d], Operadora: [%s], IP: [%s], Sinal: [%d]", 
                                                        xPortGetCoreID(),
                                                        s_connectionProperties.Operator.c_str(),
                                                        s_connectionProperties.LocalIP.toString().c_str(),
                                                        s_connectionProperties.SignalQuality);
                    delay(10000);
                }
            }
        }

        delay(1000);
    }
}

boolean mqttConnect() 
{
    ESP_LOGI("MQTT", "Conectando ao broker %s", s_mqttSettings.Broker );
    ESP_LOGI("MQTT", "Device ID: %s, Usuario: %s, Senha: %s", s_mqttSettings.DeviceId, s_mqttSettings.User, s_mqttSettings.Password );

    // Connect to MQTT Broker
    boolean status = _mqtt.connect( s_mqttSettings.DeviceId, s_mqttSettings.User, s_mqttSettings.Password );

    if (status == false) 
    {
        ESP_LOGE("MQTT", "Falha ao conectar o MQTT.");
        return false;
    }
    else
    {
        ESP_LOGI("MQTT","Conectado ao MQTT. Verificando status MQTT...");
        _mqtt.publish(topicInit, "GsmClientTest started");
        _mqtt.subscribe(topicLed);
        status = _mqtt.connected();

        ESP_LOGI("MQTT","Status: %s", status == true ? "Conectado" : "Desconectado" );
        return status;
    }
}

void monitorMQTT( void *pvParameters ) 
{
    ESP_LOGI("[CM]", "Iniciando monitoramento do MQTT");

    while( true )
    {
        if ( CommunicationManager.isConnected() == false )
        {
            ESP_LOGI("[CM]", "Aguardando conectividade...");
            delay(1000);
            continue;
        }

        if ( _mqtt.connected() == false )
        {
            ESP_LOGI("MQTT", "=== MQTT NOT CONNECTED ===" );
            
            // Reconnect every 10 seconds
            unsigned long t = millis();
            if (t - s_lastReconnectAttempt > 10000L) 
            {
                s_lastReconnectAttempt = t;
                if (mqttConnect()) 
                {
                    s_lastReconnectAttempt = 0;
                }
            }
        }

        _mqtt.loop();
        delay(100);
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int len) 
{
    ESP_LOGI("MQTT", "Message arrived [%s]: %s");

    // Only proceed if incoming message's topic matches

    // if (String(topic) == topicLed) 
    // {
    //     ledStatus = !ledStatus;
    //     digitalWrite(LED_PIN, ledStatus);
    //     mqtt.publish(topicLedStatus, ledStatus ? "1" : "0");
    // }
}

esp_err_t CommunicationManagerClass::begin( MQTTSettings_t settings )
{
    if ( s_monitorMutex == NULL )
    {
        ESP_LOGI("[CM]", "Criando mutex e atividades");

        s_monitorMutex = xSemaphoreCreateMutex();
        SerialAT.begin( 115200, SERIAL_8N1, GSM_TX, GSM_RX, false );
        //xTaskCreatePinnedToCore( monitorConnectivity, "MonitorConnectivity", 10240, NULL, 1, s_monitorTaskHandle, 0 );
        xTaskCreate( monitorConnectivity, "MonitorConnectivity", 10240, NULL, 1, s_monitorTaskHandle );
        
        if ( (strlen(settings.Broker) <= 0)     ||
             (strlen(settings.DeviceId) <= 0)   ||
             (strlen(settings.Password) <= 0)   ||
             (strlen(settings.User) <= 0)       ||
             (settings.Port <= 0 ) )
        {
            return ESP_ERR_INVALID_ARG;
        }
        
        // MQTT Broker setup
        s_mqttSettings = settings;        
        ESP_LOGI("[CM]", "Configurando servidor MQTT");
        _mqtt.setServer(settings.Broker, settings.Port );
        _mqtt.setCallback( mqttCallback );

        xTaskCreate( monitorMQTT , "MonitorMQTT", 10240, NULL, 1, s_mqttTaskHandle );
    }

    return ESP_OK;
}

esp_err_t CommunicationManagerClass::initializeMqtt()
{
    return ESP_OK;
}

bool CommunicationManagerClass::isConnected()
{
    bool result = false;

    xSemaphoreTake( s_monitorMutex , portMAX_DELAY );
        result = s_connectionProperties.IsAnyConnected;
    xSemaphoreGive( s_monitorMutex );

    return result;
}

esp_err_t CommunicationManagerClass::sendMessage( char * message , size_t size )
{
    return ESP_OK;
}

CommunicationManagerClass CommunicationManager;