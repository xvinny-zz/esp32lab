/*#include "TrackerWrapper.h"

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
volatile bool g_isConnected;
QueueHandle_t g_operationQueue;
ConnectionEventHandler OnConnectionResult;

uint8_t isClientConnected()
{
    return client.connected();
}

bool connectClient()
{
    if (!isClientConnected())
    {
        if (!client.connect(IPAddress(13, 67, 179, 120), 5001))
            return false;
        else
            return true;
    }
    else
    {
        return true;
    }
}

void disconnectClient()
{
    if (isClientConnected())
    {
        ESP_LOGD("TRACKER", "Desconectando...");
        client.stop();
    }
}

esp_err_t login()
{
    if (!connectClient())
        return ESP_ERR_NOT_SUPPORTED;

    String message;

    message = "##,imei:" + modem.getIMEI() + ",A;";

    time_t startTime = millis();
    size_t sent = client.print(message.c_str());
    ESP_LOGD("TRACKER", "Sent: %s (size: %d)", message.c_str(), sent);

    String received;

    do
    {
        received += client.readString();
        if (received.compareTo("LOAD") == 0)
        {
            ESP_LOGD("TRACKER", "Received: %s", received.c_str());
            break;
        }
    } while (client.available() && ((millis() - startTime) < 3000));

    disconnectClient();

    return ESP_OK;
}

esp_err_t heartbeat()
{
    if (!client.connect(IPAddress(13, 67, 179, 120), 5001))
    {
        return ESP_ERR_NOT_SUPPORTED;
    }

    String message;

    message = modem.getIMEI();
    size_t sent = client.print(message);
    ESP_LOGD("TRACKER", "Sent: %s (size: %d)", message.c_str(), sent);

    while (client.available())
    {
        String line = client.readString();
        ESP_LOGD("TRACKER", "Received: %s", line.c_str());
    }

    client.stop();

    return ESP_OK;
}

//esp_err_t postPosition(String id, double lat, double lng, int batteryLevel)
// {
//     HTTPClient http;

//     ESP_LOGD("MAIN", "Solicitando posicao: Id: %s, Lat: %.2f, Lon: %.2f, Bat: %d",
//              id.c_str(), lat, lng, batteryLevel);

//     //http.begin("https://gapos.centralus.cloudapp.azure.com/", rootCACertificate);
//     http.begin("https://gapos.centralus.cloudapp.azure.com/");

//     String str = "id=" + id + "&lat=" + String(lat, 6) + "&lon=" + String(lat, 6) + "&batt=" + String(batteryLevel);

//     ESP_LOGD("TRACKER", "Executando HTTP Post...");

//     int httpCode = http.POST(str);

//     ESP_LOGD("TRACKER", "HTTP Post executado! Lendo resultado...");

//     String payload = http.getString();

//     ESP_LOGD("TRACKER", "Resultado: %d\nPayload: %s", httpCode, payload.c_str());

//     http.end();
// }

esp_err_t connectNetwork()
{
    if (!modem.restart())
    {
        ESP_LOGE("TRACKER", "Falha ao restartar o modem.");
        return ESP_ERR_INVALID_STATE;
    }

    String modemInfo = modem.getModemInfo();
    ESP_LOGD("TRACKER", "Modem: %s", modemInfo.c_str());
    ESP_LOGD("TRACKER", "Aguardando rede...");

    if (!modem.waitForNetwork())
    {
        ESP_LOGE("TRACKER", "Rede nao disponivel. Reiniciando...");
        return ESP_ERR_INVALID_STATE;
    }

    if (modem.isNetworkConnected())
    {
        ESP_LOGD("TRACKER", "Rede conectada!");
    }

    return ESP_OK;
}

esp_err_t connectModem()
{
    ESP_LOGD("TRACKER", "Restartando o modem...");

    if (modem.isNetworkConnected() == false)
    {
        while (connectNetwork() != ESP_OK)
            delay(100);
    }

    ESP_LOGD("TRACKER", "Conectando ao APN [%s]...", apn);

    if (!modem.gprsConnect(apn, user, pass))
    {
        ESP_LOGE("TRACKER", "Falha ao conectar no APN [%s].", apn);
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGD("TRACKER", "Status GPRS: %s", modem.isGprsConnected() ? "Conectado" : "Nao conectado");

    String ccid = modem.getSimCCID();
    ESP_LOGD("TRACKER", "CCID: %s", ccid.c_str());

    String imei = modem.getIMEI();
    ESP_LOGD("TRACKER", "IMEI: %s", imei.c_str());

    String cop = modem.getOperator();
    ESP_LOGD("TRACKER", "Operadora: %s", cop.c_str());

    IPAddress local = modem.localIP();
    ESP_LOGD("TRACKER", "IP Local: %s", local.toString().c_str());

    ESP_LOGD("TRACKER", "Qualidade sinal: %d", modem.getSignalQuality());

    // This is NOT supported on M590
    ESP_LOGD("TRACKER", "Nivel Bateria: %d", (modem.getBattPercent()));

    // This is only supported on SIMxxx series
    ESP_LOGD("TRACKER", "Tensao Bateria: %.2f", (modem.getBattVoltage() / 1000.0F));

    // This is only supported on SIMxxx series
    String gsmLoc = modem.getGsmLocation();
    ESP_LOGD("TRACKER", "Localizacao GSM: %s", gsmLoc.c_str());

    // This is only supported on SIMxxx series
    String gsmTime = modem.getGSMDateTime(DATE_TIME);
    ESP_LOGD("TRACKER", "GSM Hora: %s", gsmTime.c_str());
    String gsmDate = modem.getGSMDateTime(DATE_DATE);
    ESP_LOGD("TRACKER", "GSM Data: %s", gsmDate.c_str());

    g_isConnected = true;

    return ESP_OK;
}

esp_err_t disconnect()
{
    ESP_LOGD("TRACKER", "Desconectando o GPRS...");

    while (modem.gprsDisconnect() == false)
    {
        ESP_LOGD("TRACKER", "Falha ao desconectar. Tentando novamente...");
        delay(1000);
    }

    g_isConnected = false;

    return ESP_OK;
}

void ConnectionMonitor(void *args)
{
    TrackerOperation_e operation;

    while (true)
    {
        if (xQueueReceive(g_operationQueue, &operation, portMAX_DELAY) == pdTRUE)
        {
            switch (operation)
            {
            case TRACKER_OP_CONNECT:
            {
                ESP_LOGD("TRACKER", "Solicitada a conexao...");
                esp_err_t result = connectModem();
                if (OnConnectionResult != NULL)
                    OnConnectionResult(result);
                break;
            }
            case TRACKER_OP_DISCONNECT:
            {
                ESP_LOGD("TRACKER", "Solicitada a desconexao...");
                disconnect();
                break;
            }
            case TRACKER_OP_LOGIN:
            {
                ESP_LOGD("TRACKER", "Solicitado o login...");
                login();
                break;
            }
            case TRACKER_OP_HEARTBEAT:
            {
                ESP_LOGD("TRACKER", "Enviando o heartbeat...");
                heartbeat();
                break;
            }
            default:
            {
                ESP_LOGE("TRACKER", "Operacao nao identificada...");
                break;
            }
            }
        }

        delay(10);
    }
}

TrackerWrapperClass::TrackerWrapperClass()
{
}

esp_err_t TrackerWrapperClass::begin()
{
    SerialAT.begin(115200, SERIAL_8N1, GSM_TX, GSM_RX, false);
    delay(500);

    g_isConnected = false;
    m_connectionStatus = CONNECTION_STATUS_DISCONNECTED;
    g_operationQueue = xQueueCreate(10, sizeof(TrackerOperation_e));

    xTaskCreate(ConnectionMonitor, "ConnectionMonitor", 10240, NULL, NULL, NULL);

    return ESP_OK;
}

esp_err_t TrackerWrapperClass::connect()
{
    if (isConnected())
        return ESP_ERR_INVALID_STATE;

    TrackerOperation_e operation = TRACKER_OP_CONNECT;
    xQueueSend(g_operationQueue, &operation, portMAX_DELAY);

    return ESP_OK;
}

esp_err_t TrackerWrapperClass::connectAsync(ConnectionEventHandler connectionEventHandler)
{
    if (m_connectionStatus != CONNECTION_STATUS_DISCONNECTED)
        return ESP_ERR_INVALID_STATE;

    OnConnectionResult = connectionEventHandler;

    TrackerOperation_e operation = TRACKER_OP_CONNECT;
    xQueueSend(g_operationQueue, &operation, portMAX_DELAY);

    return ESP_OK;
}

esp_err_t TrackerWrapperClass::disconnect()
{
    if (isConnected() == false)
        return ESP_ERR_INVALID_STATE;

    TrackerOperation_e operation = TRACKER_OP_DISCONNECT;
    xQueueSend(g_operationQueue, &operation, portMAX_DELAY);

    return ESP_OK;
}

esp_err_t TrackerWrapperClass::queuePosition()
{
    if (isConnected() == false)
        return ESP_ERR_INVALID_STATE;

    TrackerOperation_e operation = TRACKER_OP_POSITION;
    xQueueSend(g_operationQueue, &operation, portMAX_DELAY);

    return ESP_OK;
}

esp_err_t TrackerWrapperClass::queueLogin()
{
    if (isConnected() == false)
        return ESP_ERR_INVALID_STATE;

    TrackerOperation_e operation = TRACKER_OP_LOGIN;
    xQueueSend(g_operationQueue, &operation, portMAX_DELAY);

    return ESP_OK;
}

esp_err_t TrackerWrapperClass::queueHeartbeat()
{
    if (isConnected() == false)
        return ESP_ERR_INVALID_STATE;

    TrackerOperation_e operation = TRACKER_OP_HEARTBEAT;
    xQueueSend(g_operationQueue, &operation, portMAX_DELAY);

    return ESP_OK;
}

esp_err_t TrackerWrapperClass::queueGetDevices()
{
    if (isConnected() == false)
        return ESP_ERR_INVALID_STATE;

    TrackerOperation_e operation = TRACKER_OP_GET_DEVICES;
    xQueueSend(g_operationQueue, &operation, portMAX_DELAY);

    return ESP_OK;
}

bool TrackerWrapperClass::isConnected()
{
    return g_isConnected;
}

TrackerWrapperClass TrackerWrapper;*/