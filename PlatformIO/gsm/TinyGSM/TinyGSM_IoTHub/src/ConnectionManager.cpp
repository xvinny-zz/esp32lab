#include "ConnectionManager.h"

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
#endif

static volatile bool s_isMonitorRunning = false;
static TaskHandle_t * s_monitorTaskHandle;
static SemaphoreHandle_t s_monitorMutex;
static ConnectionProperties m_connectionProperties;

void monitorConnectivity( void *pvParameters ) 
{
    ESP_LOGE("[MM]", "Iniciando o monitoramento de conectividade");

    while (1) 
    {
        if ( m_connectionProperties.IsModemStarted == false )
        {
            ESP_LOGI("[MM]", "Restartando o modem");
            m_connectionProperties.IsModemStarted = _modem.restart();
            ESP_LOGI("[MM]", "Modem restartado.");
            if ( m_connectionProperties.IsModemStarted == false )
            {
                ESP_LOGE("[MM]", "Nao foi possivel reiniciar o modem GSM... Aguardando 1s para tentar novamente...");
                delay(1000);
            }
        }
        else
        {
            // Se ja esta startado, verifica a conectividade com a rede continuamente...
            if (m_connectionProperties.IsNetworkConnected == false)
            {
                ESP_LOGI("[MM]", "Nao esta conectado a rede GSM. Aguardando rede por ate 60s...");
                m_connectionProperties.IsNetworkConnected = _modem.waitForNetwork();
                if ( m_connectionProperties.IsNetworkConnected == false )
                {
                    ESP_LOGI("[MM]", "Continua nao conectado a rede GSM. Tentaremos novamente apos alguns instantes...");
                }
            }
            else
            {
                // Se ja esta conectado na rede, verifica a conectividade GPRS...
                ESP_LOGI("[MM]", "Conectado a rede GSM. Verificando a conectividade GPRS...");
                m_connectionProperties.IsGPRSConnected = _modem.isGprsConnected();
                if ( m_connectionProperties.IsGPRSConnected == false )
                {
                    ESP_LOGI("[MM]", "Tentando conectar na GPRS...");
                    m_connectionProperties.IsGPRSConnected = _modem.gprsConnect( apn, user, pass );
                    if ( m_connectionProperties.IsGPRSConnected == false )
                    {
                        ESP_LOGE("[MM]", "Falha ao tentar conectar via GPRS. Tentando novamente em alguns instantes...");
                    }
                    else
                    {
                        ESP_LOGI("[MM]", "Conectado via GPRS com sucesso...");
                    }
                }
                else
                {
                    m_connectionProperties.Operator = _modem.getOperator();
                    m_connectionProperties.LocalIP = _modem.localIP();
                    m_connectionProperties.SignalQuality = _modem.getSignalQuality();
                    ESP_LOGI("[MM]", "Operadora: [%s], IP: [%s], Sinal: [%d]", 
                                                        m_connectionProperties.Operator.c_str(),
                                                        m_connectionProperties.LocalIP.toString().c_str(),
                                                        m_connectionProperties.SignalQuality);
                    delay(10000);
                }
            }
        }

        delay(10);
    }
}

void ConnectionManagerClass::begin()
{
    if ( s_monitorMutex == NULL )
    {
        s_monitorMutex = xSemaphoreCreateMutex();
        SerialAT.begin( 115200, SERIAL_8N1, GSM_TX, GSM_RX, false );
        xTaskCreatePinnedToCore( monitorConnectivity, "MonitorConnectivity", 10240, NULL, 1, s_monitorTaskHandle, 1 );
    }
}

void ConnectionManagerClass::postMessage()
{

}

ConnectionManagerClass ConnectionManager;