#include <Arduino.h>
#include "ConnectionManager.h"
#include "Utils.h"

#define TAG_MAIN "TRACKER"
#define OPERATOR_TIM

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
#elif defined(OPERATOR_ARQIA)
const char apn[] = "iot4u.br";
const char user[] = "";
const char pass[] = "";
#else
const char apn[] = "timbrasil.br";
const char user[] = "tim";
const char pass[] = "tim";
#endif

String introduction = "Teste do GSM\n \
                      Comandos: \n \
                      -> \"POWER\": Simula pressionar o botao power\n \
                      -> \"CONNECT\": Conectar\n \
                      -> \"DISCONNECT\": Desconectar\n \
                      -> \"SET_AUTO_RECONNECT:<enabled>\": Habilitar/desabilitar o auto-reconnect\n \
                      -> \"ISCONNECTED\": Esta conectado?\n \
                      -> \"GET_CCID\": Obter o CCID do SIM\n \
                      -> \"GET_IMEI\": Retornar o IMEI do device\n \
                      -> \"GET_OPERATOR\": Retornar o OPERATOR do device\n \
                      -> \"GET_LOCALIP\": Retornar o IP LOCAL do device\n \
                      -> \"GET_SIGNAL\": Retornar o nivel de sinal\n \
                      -> \"GET_BATTERY\": Retornar o nivel de bateria\n \
                      -> \"GET_BATTERY_VOLTAGE\": Retornar a tensao da bateria\n \
                      -> \"GET_LOCATION\": Retornar a localizacao do dispositivo\n \
                      -> \"GET_DATETIME\": Retornar a data e hora GSM\n \
                      -> \"SMS:<numero>;<mensagem>\": Enviar uma SMS para um determinado numero.\n \
                      -> \"CONNECT_CLIENT:<server>;<port>;<timeout_ms>\": Conectar a um servidor remoto.\n \
                      -> \"DISCONNECT_CLIENT\": Desconectar de um servidor remoto\n \
                      -> \"ISCLIENTCONNECTED\": Verificar se o client esta conectado\n \
                      -> \"DOWNLOAD_FILE:<resource>;<size_B>;<destination_folder>;<timeout>\": Baixar um arquivo.\n \
                      -> \"HTTP_GET:<url>\": Executar o get em uma URL.\n \
                      -> \"TEST_UTILS\": Testar as rotinas uteis.\n \
                      -> \"WRITE:<hexString>\": Escrever um array de bytes na porta.\n \
                      -> \"(DESABILITADO)POSITION\": Enviar posicao padrao\n \
                      -> \"(DESABILITADO)LOGIN\": Enviar o comando de login\n \
                      -> \"(DESABILITADO)HEARTBEAT\": Enviar o comando de heartbeat\n \
                      -> \"(DESABILITADO)GETDEVICES\": Obter os dispositivos cadastrados\n \
                      -> \"(DESABILITADO)POSITION:<ID>,<LAT>,<LON>,<BAT>\": Esta conectado?\n";

void setup()
{
    Serial.begin(115200);

    ConnectionSettings_t settings;
    strcpy(settings.APN, apn);
    strcpy(settings.User, user);
    strcpy(settings.Password, pass);
    settings.AutoReconnect = false;

    if (ConnectionManager.begin(settings) != ESP_OK)
        ConnectionManager.printLastError();

    ESP_LOGI(TAG_MAIN, "%s", introduction.c_str());
}

void OnConnected(esp_err_t result)
{
    if (result == ESP_OK)
        ESP_LOGD(TAG_MAIN, "Conectado!");
    else
        ESP_LOGE(TAG_MAIN, "Erro ao conectar: %04X", result);
}

void OnClientConnected(esp_err_t result)
{
    if (result == ESP_OK)
        ESP_LOGD(TAG_MAIN, "Client Conectado!");
    else
        ESP_LOGE(TAG_MAIN, "Erro ao conectar o client: %04X", result);
}

void loop()
{
    String text = Serial.readStringUntil('\n');

    if (text.length() > 0)
    {
        if (text.compareTo("POWER") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando POWER");
            if (ConnectionManager.power() != ESP_OK)
                ESP_LOGE(TAG_MAIN, "Nao eh possivel enviar o comando POWER.");
            else
                ESP_LOGD(TAG_MAIN, "Power enviado!");
        }
        else if (text.compareTo("CONNECT") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando CONNECT");
            if (ConnectionManager.connectAsync(OnConnected) != ESP_OK)
                ESP_LOGE(TAG_MAIN, "Nao eh possivel enviar o comando CONNECT: ja conectado.");
            else
                ESP_LOGD(TAG_MAIN, "Conectando...");
        }
        else if (text.compareTo("DISCONNECT") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando DISCONNECT");
            ConnectionManager.disconnect();
        }
        else if (text.compareTo("ISCONNECTED") == 0)
        {
            ConnectionStatus_e status = ConnectionManager.getStatus();
            switch (status)
            {
            case CONNECTION_STATUS_CONNECTED:
                ESP_LOGI(TAG_MAIN, "Status: Conectado");
                break;
            case CONNECTION_STATUS_CONNECTING:
                ESP_LOGI(TAG_MAIN, "Status: Conectando");
                break;
            case CONNECTION_STATUS_DISCONNECTING:
                ESP_LOGI(TAG_MAIN, "Status: Desconectando");
                break;
            case CONNECTION_STATUS_DISCONNECTED:
                ESP_LOGI(TAG_MAIN, "Status: Desconectado");
                break;
            default:
                ESP_LOGI(TAG_MAIN, "Status: Desconhecido");
                break;
            }
        }
        else if (text.compareTo("GET_CCID") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando GET_CCID");
            String ccid;
            if (ConnectionManager.getSimCCID(&ccid) != ESP_OK)
            {
                ESP_LOGE(TAG_MAIN, "Falha ao obter o CCID.");
                ConnectionManager.printLastError();
            }
            else
                ESP_LOGD(TAG_MAIN, "CCID: %s", ccid.c_str());
            
        }
        else if (text.compareTo("GET_IMEI") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando GET_IMEI");
            String IMEI;
            if (ConnectionManager.getIMEI(&IMEI) != ESP_OK)
            {
                ESP_LOGE(TAG_MAIN, "Falha ao obter o IMEI.");
                ConnectionManager.printLastError();
            }
            else
                ESP_LOGD(TAG_MAIN, "IMEI: %s", IMEI.c_str());
            
        }
        else if (text.compareTo("GET_OPERATOR") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando GET_OPERATOR");
            String OPERATOR;
            if (ConnectionManager.getOperator(&OPERATOR) != ESP_OK)
            {
                ESP_LOGE(TAG_MAIN, "Falha ao obter o OPERATOR.");
                ConnectionManager.printLastError();
            }
            else
                ESP_LOGD(TAG_MAIN, "OPERATOR: %s", OPERATOR.c_str());
            
        }
        else if (text.compareTo("GET_LOCALIP") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando GET_LOCALIP");
            String LOCALIP;
            if (ConnectionManager.getLocalIP(&LOCALIP) != ESP_OK)
            {
                ESP_LOGE(TAG_MAIN, "Falha ao obter o IP LOCAL.");
                ConnectionManager.printLastError();
            }
            else
                ESP_LOGD(TAG_MAIN, "IP: %s", LOCALIP.c_str());
            
        }
        else if (text.compareTo("GET_SIGNAL") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando GET_SIGNAL");
            int16_t SIGNAL;
            if (ConnectionManager.getSignalQuality(&SIGNAL) != ESP_OK)
            {
                ESP_LOGE(TAG_MAIN, "Falha ao obter o nivel de sinal.");
                ConnectionManager.printLastError();
            }
            else
                ESP_LOGD(TAG_MAIN, "Sinal: %d", SIGNAL);
            
        }
        else if (text.compareTo("GET_BATTERY") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando GET_BATTERY");
            int8_t battery;
            if (ConnectionManager.getBatteryLevel(&battery) != ESP_OK)
            {
                ESP_LOGE(TAG_MAIN, "Falha ao obter o nivel de bateria.");
                ConnectionManager.printLastError();
            }
            else
                ESP_LOGD(TAG_MAIN, "Bateria: %d%%", battery);
            
        }
        else if (text.compareTo("GET_BATTERY_VOLTAGE") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando GET_BATTERY_VOLTAGE");
            double voltage;
            if (ConnectionManager.getBatteryVoltage(&voltage) != ESP_OK)
            {
                ESP_LOGE(TAG_MAIN, "Falha ao obter a tensao de bateria.");
                ConnectionManager.printLastError();
            }
            else
                ESP_LOGD(TAG_MAIN, "Bateria: %.2fV", voltage);
            
        }
        else if (text.compareTo("GET_LOCATION") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando GET_LOCATION");
            String location;
            if (ConnectionManager.getGsmLocation(&location) != ESP_OK)
            {
                ESP_LOGE(TAG_MAIN, "Falha ao obter a localizacao por GSM.");
                ConnectionManager.printLastError();
            }
            else
                ESP_LOGD(TAG_MAIN, "Localizacao GSM: %s", location.c_str());
        }
        else if (text.compareTo("GET_DATETIME") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando GET_DATETIME");
            String datetime;
            if (ConnectionManager.getDateTime(&datetime) != ESP_OK)
            {
                ESP_LOGE(TAG_MAIN, "Falha ao obter a data/hora por GSM.");
                ConnectionManager.printLastError();
            }
            else
                ESP_LOGD(TAG_MAIN, "Data/Hora: %s", datetime.c_str());
        }
        else if (text.compareTo("DISCONNECT_CLIENT") == 0)
        {
            ESP_LOGI(TAG_MAIN, "Enviando comando DISCONNECT_CLIENT");
            String datetime;
            if (ConnectionManager.disconnectClient() != ESP_OK)
            {
                ESP_LOGE(TAG_MAIN, "Falha ao desconectar o client.");
                ConnectionManager.printLastError();
            }
            else
                ESP_LOGD(TAG_MAIN, "Client desconectado!");
        }
        else if (text.compareTo("ISCLIENTCONNECTED") == 0)
        {
            if (ConnectionManager.isClientConnected())   
                ESP_LOGI(TAG_MAIN, "Client conectado.");
            else
                ESP_LOGE(TAG_MAIN, "Client desconectado!");
        }
        else
        {
            if (text.startsWith(String("SMS:")))
            {
                char separator = ';';
                text.remove(0, (text.indexOf(':') + 1));
                if (text.length() <= 0)
                    ESP_LOGE(TAG_MAIN, "Comando invalido");
                else
                {
                    String number = text.substring(0, text.indexOf(separator));
                    text.remove(0, (text.indexOf(separator) + 1));
                    if (text.length() <= 0)
                        ESP_LOGE(TAG_MAIN, "Comando invalido");
                    else
                    {
                        String message = text;
                        if (ConnectionManager.sendSMS(number, message) != ESP_OK)
                        {
                            ESP_LOGE(TAG_MAIN, "Falha ao enviar uma SMS.");
                            ConnectionManager.printLastError();
                        }
                        else
                            ESP_LOGD(TAG_MAIN, "Mensagem enviada!");
                    }
                }
            }
            else if (text.startsWith(String("CONNECT_CLIENT:")))
            {
                char separator = ';';
                text.remove(0, (text.indexOf(':') + 1));
                if (text.length() <= 0)
                    ESP_LOGE(TAG_MAIN, "Comando invalido");
                else
                {
                    String server = text.substring(0, text.indexOf(separator));
                    text.remove(0, (text.indexOf(separator) + 1));
                    if (text.length() <= 0)
                        ESP_LOGE(TAG_MAIN, "Comando invalido");
                    else
                    {
                        String port = text.substring(0, text.indexOf(separator));
                        text.remove(0, (text.indexOf(separator) + 1));
                        if (text.length() <= 0)
                            ESP_LOGE(TAG_MAIN, "Comando invalido");
                        else
                        {
                            String timeout = text;
                            if (ConnectionManager.connectClientAsync(OnClientConnected, server.c_str(), atoi(port.c_str()), atoi(timeout.c_str())) != ESP_OK)
                            {
                                ESP_LOGE(TAG_MAIN, "Falha ao conectar o client.");
                                ConnectionManager.printLastError();
                            }
                            else
                                ESP_LOGD(TAG_MAIN, "Client conectado!");
                        }
                    }
                }
            }
            else if (text.startsWith(String("DOWNLOAD_FILE:")))
            {
                ESP_LOGI(TAG_MAIN, "Enviando comando DOWNLOAD_FILE");
                char separator = ';';
                text.remove(0, (text.indexOf(':') + 1));
                if (text.length() <= 0)
                    ESP_LOGE(TAG_MAIN, "Comando invalido");
                else
                {
                    String resource = text.substring(0, text.indexOf(separator));
                    text.remove(0, (text.indexOf(separator) + 1));
                    if (text.length() <= 0)
                        ESP_LOGE(TAG_MAIN, "Comando invalido");
                    else
                    {
                        String fileSize = text.substring(0, text.indexOf(separator));
                        text.remove(0, (text.indexOf(separator) + 1));
                        if (text.length() <= 0)
                            ESP_LOGE(TAG_MAIN, "Comando invalido");
                        else
                        {
                            String destinationFolder = text.substring(0, text.indexOf(separator));
                            text.remove(0, (text.indexOf(separator) + 1));
                            if (text.length() <= 0)
                                ESP_LOGE(TAG_MAIN, "Comando invalido");
                            else
                            {
                                String timeout = text;
                                if (ConnectionManager.downloadFile(resource.c_str(), atol(fileSize.c_str()), destinationFolder.c_str(), atol(timeout.c_str())) != ESP_OK)
                                {
                                    ESP_LOGE(TAG_MAIN, "Falha ao conectar o client.");
                                    ConnectionManager.printLastError();
                                }
                                else
                                    ESP_LOGD(TAG_MAIN, "Arquivo baixado!");
                            }
                        }
                    }
                }
            }
            else if (text.startsWith(String("HTTP_GET:")))
            {
                ESP_LOGI(TAG_MAIN, "Enviando comando HTTP_GET");
                text.remove(0, (text.indexOf(':') + 1));
                if (text.length() <= 0)
                    ESP_LOGE(TAG_MAIN, "Comando invalido");
                else
                {
                    String result = "";
                    if (ConnectionManager.httpGet(text.c_str(), &result, 10000 ) != ESP_OK)
                        ESP_LOGE(TAG_MAIN, "Falha ao executar o GET.");
                    else
                        ESP_LOGD(TAG_MAIN, "Resultado do GET: %s", result.c_str());

                }
            }
            else if (text.startsWith(String("WRITE:")))
            {
                ESP_LOGI(TAG_MAIN, "Enviando comando WRITE");
                text.remove(0, (text.indexOf(':') + 1));
                if (text.length() <= 0)
                    ESP_LOGE(TAG_MAIN, "Comando invalido");
                else
                {
                    String result = "";
                    uint8_t *buffer = Utils.fromHexString(text.c_str());
                    size_t written = 0;
                    if (ConnectionManager.write(buffer, text.length() / 2, &written ) != ESP_OK)
                        ESP_LOGE(TAG_MAIN, "Falha ao executar o WRITE. %s", ConnectionManager.getLastError().Message);
                    else
                        ESP_LOGD(TAG_MAIN, "Resultado do WRITE: %d", written);

                }
            }
            else if (text.startsWith(String("SET_AUTO_RECONNECT:")))
            {
                ESP_LOGI(TAG_MAIN, "Enviando comando SET_AUTO_RECONNECT");
                text.remove(0, (text.indexOf(':') + 1));
                if (text.length() <= 0)
                    ESP_LOGE(TAG_MAIN, "Comando invalido");
                else
                {
                    int autoReconnect = atoi(text.c_str());
                    ConnectionManager.setAutoReconnect(autoReconnect == 1);
                    ESP_LOGD(TAG_MAIN, "Auto Reconnect configurado para: %d", autoReconnect);
                }
            }
        }
        
        // else if (text.compareTo("POSITION") == 0)
        // {
        //     ESP_LOGI(TAG_MAIN, "Enviando comando POSITION");
        //     if (TrackerWrapper.queuePosition() != ESP_OK)
        //         ESP_LOGE(TAG_MAIN, "Nao eh possivel enviar o comando POSITION.");
        // }
        // else if (text.compareTo("LOGIN") == 0)
        // {
        //     ESP_LOGI(TAG_MAIN, "Enviando comando LOGIN");
        //     if (TrackerWrapper.queueLogin() != ESP_OK)
        //         ESP_LOGE(TAG_MAIN, "Nao eh possivel enviar o comando LOGIN.");
        // }
        // else if (text.compareTo("HEARTBEAT") == 0)
        // {
        //     ESP_LOGI(TAG_MAIN, "Enviando comando HEARTBEAT");
        //     if (TrackerWrapper.queueHeartbeat() != ESP_OK)
        //         ESP_LOGE(TAG_MAIN, "Nao eh possivel enviar o comando HEARTBEAT.");
        // }
        // else if (text.compareTo("GETDEVICES") == 0)
        // {
        //     ESP_LOGI(TAG_MAIN, "Enviando comando GETDEVICES");
        //     if (TrackerWrapper.queueGetDevices() != ESP_OK)
        //         ESP_LOGE(TAG_MAIN, "Nao eh possivel enviar o comando GETDEVICES.");
        // }
        // else
        // {
        //     if (text.startsWith(String("POSITION:")))
        //     {
        //         text.remove(0, (text.indexOf(':') + 1));
        //         if (text.length() <= 0)
        //             ESP_LOGE("MAIN", "Comando invalido");
        //         else
        //         {
        //             String id = text.substring(0, text.indexOf(','));
        //             text.remove(0, (text.indexOf(',') + 1));
        //             if (text.length() <= 0)
        //                 ESP_LOGE("MAIN", "Comando invalido");
        //             else
        //             {
        //                 double latitude = atof(text.substring(0, text.indexOf(',')).c_str());
        //                 text.remove(0, (text.indexOf(',') + 1));
        //                 if (text.length() <= 0)
        //                     ESP_LOGE("MAIN", "Comando invalido");
        //                 else
        //                 {
        //                     double longitude = atof(text.substring(0, text.indexOf(',')).c_str());
        //                     text.remove(0, (text.indexOf(',') + 1));
        //                     if (text.length() <= 0)
        //                         ESP_LOGE("MAIN", "Comando invalido");
        //                     else
        //                     {
        //                         int battery = atoi(text.c_str());
        //                         ESP_LOGD("MAIN", "Solicitando posicao: Id: %s, Lat: %.2f, Lon: %.2f, Bat: %d",
        //                                  id.c_str(), latitude, longitude, battery);
                                
        //                         //TrackerWrapper.postPosition(id, latitude, longitude, battery);
        //                     }
        //                 }
        //             }
        //         }
        //     }
        //     else
        //         ESP_LOGI(TAG_MAIN, "Comando nao reconhecido. Texto: %s", text.c_str());
        // }
    }

    delay(10);
}