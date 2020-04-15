/**
 * @file ConnectionManager.cpp
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief 
 * @version 0.1
 * @date 21/02/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
////////////////////////////////////////////////////////////////////////
//                          INCLUDES
////////////////////////////////////////////////////////////////////////
#include "ConnectionManager.h"

//#define SDSPEED SD_SCK_MHZ(20) // result: 101 tracks in 771 ms
#define DEBUG_GSM

////////////////////////////////////////////////////////////////////////
//                          PROTÓTIPOS
////////////////////////////////////////////////////////////////////////
esp_err_t operationConnect();
void operationDisconnect();
esp_err_t connectNetwork();
void setConnectionStatus(ConnectionStatus_e status);
ConnectionStatus_e getConnectionStatus();
esp_err_t setLastError(int id, const char *message);
esp_err_t operationConnectClient(char *parameters);
void processOperationQueue();
void doAutoReconnect();

////////////////////////////////////////////////////////////////////////
//                          VARIÁVEIS
////////////////////////////////////////////////////////////////////////
volatile bool g_isConnectionManagerConnected = false;
volatile ConnectionStatus_e g_connectionStatus;
QueueHandle_t g_connectionOperationQueue;
static ConnectionSettings_t g_connectionSettings;
static ConnectionError_t m_lastError;
SemaphoreHandle_t m_lock;
TaskHandle_t m_operationMonitorHandle;
String g_currentServer;
SdFat SD;
time_t g_lastReconnectCheck;

#ifdef DEBUG_GSM
#include <StreamDebugger.h>
#define SerialMon Serial
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);

////////////////////////////////////////////////////////////////////////
//                          EVENTOS
////////////////////////////////////////////////////////////////////////
ConnectionEventHandler OnConnectResult;
ConnectClientEventHandler OnConnectClientResult;

////////////////////////////////////////////////////////////////////////
//                          FUNÇÕES ESTÁTICAS
////////////////////////////////////////////////////////////////////////
/**
 * @brief Task que monitora as operações assíncronas.
 * @param void *args Argumentos da task.
 * 
 */
void OperationMonitor(void *args)
{
    while (true)
    {
        processOperationQueue();
        doAutoReconnect();
        delay(10);
    }
}

void processOperationQueue()
{
    AsyncOperationData_t operation;

    if (xQueueReceive(g_connectionOperationQueue, &operation, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        switch (operation.Operation)
        {
        case CON_OP_CONNECT:
        {
            ESP_LOGD(TAG_CONNMGR, "Conectando o modem...");
            esp_err_t result = operationConnect();
            if (OnConnectResult != NULL)
                OnConnectResult(result);
            break;
        }
        case CON_OP_CONNECT_CLIENT:
        {
            ESP_LOGD(TAG_CONNMGR, "Conectando o client...");
            esp_err_t result = operationConnectClient(operation.Parameters);
            if (OnConnectClientResult != NULL)
                OnConnectClientResult(result);
            break;
        }
        default:
            break;
        }
    }
}

void doAutoReconnect()
{
    if ((g_connectionSettings.AutoReconnect == false) || (millis() - g_lastReconnectCheck) < 60000)
        return;

    ConnectionStatus_e result;

    xSemaphoreTake(m_lock, portMAX_DELAY);
    result = g_connectionStatus;
    xSemaphoreGive(m_lock);

    if (result == CONNECTION_STATUS_CONNECTED)
    {
        if ((modem.isGprsConnected() == false) || (modem.isNetworkConnected() == false))
        {
            ESP_LOGD(TAG_CONNMGR, "Reconectando...");
            operationConnect();
        }
    }

    g_lastReconnectCheck = millis();
}

/**
 * @brief Retornar o status de conectividade.
 * 
 * @return ConnectionStatus_e  Status de conectividade.
 */
ConnectionStatus_e getConnectionStatus()
{
    ConnectionStatus_e result;

    ESP_LOGD(TAG_CONNMGR, "Lendo status conexao.");
    xSemaphoreTake(m_lock, portMAX_DELAY);
    result = g_connectionStatus;
    xSemaphoreGive(m_lock);

    ESP_LOGD(TAG_CONNMGR, "Status lido.");

    if (result == CONNECTION_STATUS_CONNECTED)
    {
        ESP_LOGD(TAG_CONNMGR, "Ja conectado.");
        if ((modem.isGprsConnected() == false) || (modem.isNetworkConnected() == false))
        {
            ESP_LOGD(TAG_CONNMGR, "A conexao caiu.");
            result = CONNECTION_STATUS_DISCONNECTED;
            setConnectionStatus(result);
        }
    }

    return result;
}

/**
 * @brief Configurar o status de conectividade.
 * 
 * @param status Status de conectividade.
 */
void setConnectionStatus(ConnectionStatus_e status)
{
    ESP_LOGD(TAG_CONNMGR, "Configurando o ConnectionStatus: %02X", (uint8_t)status);

    xSemaphoreTake(m_lock, portMAX_DELAY);
    g_connectionStatus = status;
    xSemaphoreGive(m_lock);
}

/**
 * @brief Configurar o último erro ocorrido.
 * 
 * @param id Código do erro.
 * @param message Mensagem do erro.
 */
esp_err_t setLastError(int id, const char *message)
{
    ESP_LOGE(TAG_CONNMGR, "Error: %d (%s)", id, message);

    xSemaphoreTake(m_lock, portMAX_DELAY);
    m_lastError.Code = id;
    memset(m_lastError.Message, 0, sizeof(m_lastError.Message));
    strcpy(m_lastError.Message, message);
    xSemaphoreGive(m_lock);

    return id;
}

/**
 * @brief Operação de conexão do modem.
 * 
 * @return esp_err_t Resultado da operação.
 */
esp_err_t operationConnect()
{
    ESP_LOGD(TAG_CONNMGR, "[%lu]Conectando", millis());

    if (modem.isNetworkConnected() == false)
    {
        while (connectNetwork() != ESP_OK)
            delay(100);
    }

    ESP_LOGD(TAG_CONNMGR, "Conectando ao APN [%s]...", g_connectionSettings.APN);
    if (!modem.gprsConnect(g_connectionSettings.APN, g_connectionSettings.User, g_connectionSettings.Password))
    {
        ESP_LOGE("TRACKER", "Falha ao conectar no APN [%s].", g_connectionSettings.APN);
        setConnectionStatus(CONNECTION_STATUS_DISCONNECTED);
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGD(TAG_CONNMGR, "Status GPRS: %s", modem.isGprsConnected() ? "Conectado" : "Nao conectado");

    String ccid = modem.getSimCCID();
    ESP_LOGD(TAG_CONNMGR, "CCID: %s", ccid.c_str());

    String imei = modem.getIMEI();
    ESP_LOGD(TAG_CONNMGR, "IMEI: %s", imei.c_str());

    String cop = modem.getOperator();
    ESP_LOGD(TAG_CONNMGR, "Operadora: %s", cop.c_str());

    IPAddress local = modem.localIP();
    ESP_LOGD(TAG_CONNMGR, "IP Local: %s", local.toString().c_str());

    ESP_LOGD(TAG_CONNMGR, "Qualidade sinal: %d", modem.getSignalQuality());

    // This is NOT supported on M590
    ESP_LOGD(TAG_CONNMGR, "Nivel Bateria: %d", (modem.getBattPercent()));

    // This is only supported on SIMxxx series
    ESP_LOGD(TAG_CONNMGR, "Tensao Bateria: %.2f", (modem.getBattVoltage() / 1000.0F));

    // This is only supported on SIMxxx series
    String gsmLoc = modem.getGsmLocation();
    ESP_LOGD(TAG_CONNMGR, "Localizacao GSM: %s", gsmLoc.c_str());

    // This is only supported on SIMxxx series
    String gsmTime = modem.getGSMDateTime(DATE_TIME);
    ESP_LOGD(TAG_CONNMGR, "GSM Hora: %s", gsmTime.c_str());
    String gsmDate = modem.getGSMDateTime(DATE_DATE);
    ESP_LOGD(TAG_CONNMGR, "GSM Data: %s", gsmDate.c_str());

    setConnectionStatus(CONNECTION_STATUS_CONNECTED);

    return ESP_OK;
}

esp_err_t operationConnectClient(char *parameters)
{
    char *pch;
    const char *separator = ";";
    int count = 0;
    char *server;
    int port = 0;
    time_t timeout_s = 0;

    ESP_LOGD(TAG_CONNMGR, "Connect Client Parameters: %s", parameters);

    pch = strtok(parameters, separator);
    while (pch != NULL)
    {
        if (count == 0)
        {
            server = (char *)malloc(sizeof(char) * (strlen(pch) + 1));
            strcpy(server, pch);
        }
        else if (count == 1)
        {
            port = atoi(pch);
        }
        else if (count == 2)
        {
            timeout_s = atol(pch);
        }
        else
        {
            break;
        }

        pch = strtok(NULL, separator);
        count++;
    }

    ESP_LOGD(TAG_CONNMGR, "Connect client (server: %s, port: %lu, timeout: %d)", server, port, timeout_s);

    if (count < 2)
        return setLastError(ESP_ERR_INVALID_ARG, "Falha ao conectar o cliente: Faltam argumentos.");

    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao conectar o cliente: Nao conectado a rede.");

    g_currentServer = String(server);

    ESP_LOGD(TAG_CONNMGR, "Conectando no servidor...", server, port, timeout_s);
    if (!client.connect(server, port, timeout_s))
        return setLastError(ESP_FAIL, "Falha ao conectar no servidor");

    return ESP_OK;
}

/**
 * @brief Operação de desconexão do modem.
 * 
 */
void operationDisconnect()
{
    ESP_LOGD(TAG_CONNMGR, "Desconectando o GPRS...");

    while (modem.gprsDisconnect() == false)
    {
        ESP_LOGD(TAG_CONNMGR, "Falha ao desconectar. Tentando novamente...");
        delay(1000);
    }

    setConnectionStatus(CONNECTION_STATUS_DISCONNECTED);
}

/**
 * @brief Conectar o modem à rede.
 * 
 * @return esp_err_t Resultado da operação.
 */
esp_err_t connectNetwork()
{
    if (!modem.restart())
    {
        // 100 - "Falha ao restartar o modem"
        setLastError(CON_ERR_0001, CON_ERR_MSG_0001);
        return ESP_ERR_INVALID_STATE;
    }

    String modemInfo = modem.getModemInfo();
    ESP_LOGD(TAG_CONNMGR, "Modem: %s", modemInfo.c_str());
    ESP_LOGD(TAG_CONNMGR, "Aguardando rede...");

    if (!modem.waitForNetwork())
    {
        // 101 - "Rede nao disponivel"
        setLastError(CON_ERR_0002, CON_ERR_MSG_0002);
        return ESP_ERR_INVALID_STATE;
    }

    if (!modem.isNetworkConnected())
    {
        // 102 - "Nao conectado a rede"
        setLastError(CON_ERR_0003, CON_ERR_MSG_0003);
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGD(TAG_CONNMGR, "Rede conectada!");

    return ESP_OK;
}

/**
 * @brief Inicializar o objeto ConnectionManager.
 * 
 */
ConnectionManagerClass::ConnectionManagerClass()
{
    m_isStarted = false;
}

/**
 * @brief Inicializar o ConnectionManager.
 * 
 * @param settings Configurações de rede.
 */
esp_err_t ConnectionManagerClass::begin(ConnectionSettings_t settings)
{
    // "[begin] (APN:%s,User:%s,Password:%s)"
    ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0001, settings.APN, settings.User, settings.Password);

    g_lastReconnectCheck = 0;
    g_currentServer = "";
    g_connectionOperationQueue = xQueueCreate(20, sizeof(AsyncOperationData_t));
    m_lock = xSemaphoreCreateMutex();
    if (m_lock == NULL)
        return ESP_ERR_INVALID_STATE;

    if (strlen(settings.APN) <= 0)
    {
        setLastError(1, "[begin]Invalid APN");
        return ESP_ERR_INVALID_STATE;
    }

    memset(g_connectionSettings.APN, 0, sizeof(g_connectionSettings.APN));
    memset(g_connectionSettings.User, 0, sizeof(g_connectionSettings.User));
    memset(g_connectionSettings.Password, 0, sizeof(g_connectionSettings.Password));
    strcpy(g_connectionSettings.APN, settings.APN);
    strcpy(g_connectionSettings.User, settings.User);
    strcpy(g_connectionSettings.Password, settings.Password);

    SerialAT.begin(115200, SERIAL_8N1, GSM_TX, GSM_RX, false);
    //SerialAT.begin(9600, SERIAL_8N1, GSM_TX, GSM_RX, false);
    delay(500);

    xTaskCreate(OperationMonitor, "OperationMonitor", 10240, NULL, NULL, &m_operationMonitorHandle);

    setConnectionStatus(CONNECTION_STATUS_DISCONNECTED);

    if (!SD.begin(GPIO_NUM_5, SD_SCK_MHZ(20)))
    {
        // Falha ao montar o SD card.
        ESP_LOGE(TAG_ENGINE, "Falha ao montar o SD card.");
        return ESP_ERR_INVALID_STATE;
    }

    return ESP_OK;
}

esp_err_t ConnectionManagerClass::connectAsync(ConnectionEventHandler handler, bool autoReconnect)
{
    ESP_LOGD(TAG_CONNMGR, "Conectando async...");

    if (getConnectionStatus() != CONNECTION_STATUS_DISCONNECTED)
        return ESP_ERR_INVALID_STATE;

    OnConnectResult = handler;

    AsyncOperationData_t data;
    data.Operation = CON_OP_CONNECT;
    data.Time = millis();

    ESP_LOGD(TAG_CONNMGR, "Configurando auto reconnect...");
    setAutoReconnect(autoReconnect);

    ESP_LOGD(TAG_CONNMGR, "Configurando connection status...");
    setConnectionStatus(CONNECTION_STATUS_CONNECTING);

    ESP_LOGD(TAG_CONNMGR, "Enviando comando para conectar...");

    xQueueSend(g_connectionOperationQueue, &data, portMAX_DELAY);

    ESP_LOGD(TAG_CONNMGR, "Comando enviado!");

    return ESP_OK;
}

void ConnectionManagerClass::disconnect()
{
    if (getConnectionStatus() == CONNECTION_STATUS_CONNECTED)
    {
        setAutoReconnect(false);
        operationDisconnect();
    }
}

ConnectionError_t ConnectionManagerClass::getLastError()
{
    ConnectionError_t error;

    xSemaphoreTake(m_lock, portMAX_DELAY);
    error.Code = m_lastError.Code;
    memset(error.Message, 0, sizeof(error.Message));
    strcpy(error.Message, m_lastError.Message);
    xSemaphoreGive(m_lock);

    return error;
}

void ConnectionManagerClass::printLastError()
{
    ESP_LOGD(TAG_CONNMGR, "Erro [%d]: %s", m_lastError.Code, m_lastError.Message);
}

/**
 * @brief Retornar o status de conectividade atual.
 * 
 * @return ConnectionStatus_e Status de conectividade.
 */
ConnectionStatus_e ConnectionManagerClass::getStatus()
{
    return getConnectionStatus();
}

/**
 * @brief Retornar se o client está conectado.
 * 
 * @return bool Status de conectividade do client.
 */
bool ConnectionManagerClass::isClientConnected()
{
    return client.connected();
}

esp_err_t ConnectionManagerClass::getSimCCID(String *result)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
    {
        String message = "Falha ao obter SIM CCID: Nao conectado. Status: " + String((uint8_t)status, HEX);
        setLastError(ESP_ERR_INVALID_STATE, message.c_str());
        return ESP_ERR_INVALID_STATE;
    }

    *result = modem.getSimCCID();

    return ESP_OK;
}

esp_err_t ConnectionManagerClass::getIMEI(String *result)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
    {
        String message = "Falha ao obter IMEI: Nao conectado. Status: " + String((uint8_t)status, HEX);
        setLastError(ESP_ERR_INVALID_STATE, message.c_str());
        return ESP_ERR_INVALID_STATE;
    }

    *result = modem.getIMEI();

    return ESP_OK;
}

/**
 * @brief Retornar a operadora.
 * 
 * @param result Operadora.
 * @return esp_err_t Resultado da operação.
 */
esp_err_t ConnectionManagerClass::getOperator(String *result)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
    {
        String message = "Falha ao obter Operadora: Nao conectado. Status: " + String((uint8_t)status, HEX);
        setLastError(ESP_ERR_INVALID_STATE, message.c_str());
        return ESP_ERR_INVALID_STATE;
    }

    *result = modem.getOperator();

    return ESP_OK;
}

/**
 * @brief Retornar o IP local.
 * 
 * @param result IP local.
 * @return esp_err_t Resultado da operação.
 */
esp_err_t ConnectionManagerClass::getLocalIP(String *result)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
    {
        String message = "Falha ao obter IP Local: Nao conectado. Status: " + String((uint8_t)status, HEX);
        setLastError(ESP_ERR_INVALID_STATE, message.c_str());
        return ESP_ERR_INVALID_STATE;
    }

    *result = modem.getLocalIP();

    return ESP_OK;
}

/**
 * @brief Retornar o nivel de sinal.
 * 
 * @param result Nivel de sinal.
 * @return esp_err_t Resultado da operação.
 */
esp_err_t ConnectionManagerClass::getSignalQuality(int16_t *result)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
    {
        String message = "Falha ao obter o nivel de sinal: Nao conectado. Status: " + String((uint8_t)status, HEX);
        setLastError(ESP_ERR_INVALID_STATE, message.c_str());
        return ESP_ERR_INVALID_STATE;
    }

    *result = modem.getSignalQuality();

    return ESP_OK;
}

/**
 * @brief Retornar o nivel de bateria
 * 
 * @param result Nivel de bateria, em %.
 * @return esp_err_t Resultado da operação.
 */
esp_err_t ConnectionManagerClass::getBatteryLevel(int8_t *result)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
    {
        String message = "Falha ao obter o nivel de bateria (%): Nao conectado. Status: " + String((uint8_t)status, HEX);
        setLastError(ESP_ERR_INVALID_STATE, message.c_str());
        return ESP_ERR_INVALID_STATE;
    }

    *result = modem.getBattPercent();

    return ESP_OK;
}

/**
 * @brief Retornar a tensão da bateria
 * 
 * @param result Tensão bateria, em V.
 * @return esp_err_t Resultado da operação.
 */
esp_err_t ConnectionManagerClass::getBatteryVoltage(double *result)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
    {
        String message = "Falha ao obter a tensão da bateria (%): Nao conectado. Status: " + String((uint8_t)status, HEX);
        setLastError(ESP_ERR_INVALID_STATE, message.c_str());
        return ESP_ERR_INVALID_STATE;
    }

    *result = modem.getBattVoltage() / 1000.0F;

    return ESP_OK;
}

/**
 * @brief Retornar a localização por GSM.
 * 
 * @param result Localização.
 * @return esp_err_t Resultado da operação.
 */
esp_err_t ConnectionManagerClass::getGsmLocation(String *result)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
    {
        String message = "Falha ao obter a localizacao: Nao conectado. Status: " + String((uint8_t)status, HEX);
        setLastError(ESP_ERR_INVALID_STATE, message.c_str());
        return ESP_ERR_INVALID_STATE;
    }

    *result = modem.getGsmLocation();

    return ESP_OK;
}

/**
 * @brief Retornar a data e hora da rede GSM.
 * 
 * @param result Data e hora.
 * @return esp_err_t Resultado da operação.
 */
esp_err_t ConnectionManagerClass::getDateTime(String *result)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
    {
        String message = "Falha ao obter a data e hora: Nao conectado. Status: " + String((uint8_t)status, HEX);
        setLastError(ESP_ERR_INVALID_STATE, message.c_str());
        return ESP_ERR_INVALID_STATE;
    }

    *result = modem.getGSMDateTime(DATE_FULL);

    return ESP_OK;
}

/**
 * @brief Enviar uma SMS.
 * 
 * @param number Numero que a SMS sera enviada.
 * @param text Mensagem a ser enviada.
 * @return esp_err_t Resultado da operacao.
 */
esp_err_t ConnectionManagerClass::sendSMS(String number, String text)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
    {
        String message = "Falha ao obter a temperatura do modulo: Nao conectado. Status: " + String((uint8_t)status, HEX);
        setLastError(ESP_ERR_INVALID_STATE, message.c_str());
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGD(TAG_CONNMGR, "SMD de [%s]: [%s]", number.c_str(), text.c_str());

    if (modem.sendSMS(number, text))
        return ESP_OK;
    else
        return ESP_FAIL;
}

esp_err_t ConnectionManagerClass::connectClient(const char *server, int port, int timeout_s)
{
    ESP_LOGD(TAG_CONNMGR, "Connect client (server: %s, port: %lu, timeout: %d)", server, port, timeout_s);

    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao conectar o cliente: Nao conectado a rede.");

    if (isClientConnected())
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao conectar ao client: Ja conectado");

    g_currentServer = String(server);

    if (!client.connect(server, port, timeout_s))
        return setLastError(ESP_FAIL, "Falha ao conectar no servidor");

    return ESP_OK;
}

esp_err_t ConnectionManagerClass::connectClientAsync(ConnectClientEventHandler handler, const char *server, int port, int timeout_s)
{
    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao conectar o cliente: Nao conectado a rede.");

    if (isClientConnected())
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao conectar ao client: Ja conectado");

    ESP_LOGD(TAG_CONNMGR, "Connect client (server: %s, port: %lu, timeout: %d)", server, port, timeout_s);

    OnConnectClientResult = handler;

    AsyncOperationData_t data;
    data.Operation = CON_OP_CONNECT_CLIENT;
    memset(data.Parameters, 0, sizeof(data.Parameters));
    strcpy(data.Parameters, server);
    strcat(data.Parameters, ";");
    strcat(data.Parameters, String(port).c_str());
    strcat(data.Parameters, ";");
    strcat(data.Parameters, String(timeout_s).c_str());
    data.Time = millis();

    xQueueSend(g_connectionOperationQueue, &data, portMAX_DELAY);

    return ESP_OK;
}

esp_err_t ConnectionManagerClass::disconnectClient()
{
    if (client.connected())
        client.stop();
    else
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao desconectar o client: Ja estava desconectado.");

    return ESP_OK;
}

esp_err_t ConnectionManagerClass::downloadFile(const char *resource, uint32_t fileSize, const char *destinationFile, time_t timeout)
{
    ESP_LOGD(TAG_CONNMGR, "Download File (name: %s, size: %lu, destination: %s, timeout: %lu", resource, fileSize, destinationFile, timeout);

    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao baixar o arquivo: Nao conectado a rede.");

    if (!client.connected())
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao baixar o arquivo: Nao conectado ao servidor.");

    String message = String("GET ") + resource + " HTTP/1.0\r\n";
    // Make a HTTP GET request:
    client.print(message.c_str());
    ESP_LOGD(TAG_CONNMGR, "%s", message.c_str());
    message = String("Host: ") + g_currentServer.c_str() + "\r\n";
    client.print(message.c_str());
    ESP_LOGD(TAG_CONNMGR, "%s", message.c_str());
    message = "Connection: close\r\n\r\n";
    client.print(message.c_str());
    ESP_LOGD(TAG_CONNMGR, "%s", message.c_str());

    long startTime = millis();
    while (client.available() == 0)
    {
        if ((millis() - startTime) > timeout)
        {
            ESP_LOGE(TAG_CONNMGR, ">>> Client Timeout !");
            return ESP_FAIL;
        }
    }

    ESP_LOGD(TAG_CONNMGR, "Reading response header");
    uint32_t contentLength = fileSize;

    while (client.available())
    {
        String line = client.readStringUntil('\n');
        line.trim();
        ESP_LOGD(TAG_CONNMGR, "%s", line.c_str()); // Uncomment this to show response header
        line.toLowerCase();

        if (line.startsWith("content-length:"))
        {
            contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
        }
        else if (line.length() == 0)
        {
            break;
        }
    }

    ESP_LOGD(TAG_CONNMGR, "Reading response data");
    startTime = millis();
    uint32_t readLength = 0;

    unsigned long timeElapsed = millis();
    String incoming = "";

    File file = SD.open(destinationFile, FILE_WRITE);

    while (readLength < contentLength && client.connected() && (millis() - startTime) < timeout)
    {
        while (client.available())
        {
            uint8_t c = client.read();
            if (readLength < 10240)
                incoming.concat((char)c);

            //Serial.print((char)c);       // Uncomment this to show data
            file.print((char)c);
            readLength++;

            if (readLength % (contentLength / 13) == 0)
                ESP_LOGD(TAG_CONNMGR, "Baixado %.2f%%", (100.0 * readLength) / contentLength);

            startTime = millis();
        }
    }

    file.close();

    timeElapsed = millis() - timeElapsed;
    float duration = float(timeElapsed) / 1000;

    ESP_LOGD(TAG_CONNMGR, "Content-Length: %lu", contentLength);
    ESP_LOGD(TAG_CONNMGR, "Actually read: %lu", readLength);
    ESP_LOGD(TAG_CONNMGR, "Duration: %.2fs", duration);
    ESP_LOGD(TAG_CONNMGR, "Content: %s", incoming.c_str());

    return ESP_OK;
}

esp_err_t ConnectionManagerClass::httpGet(const char *partialUrl, String *result, int timeout)
{
    ESP_LOGD(TAG_CONNMGR, "HTTP GET (url: %s", partialUrl);

    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
        return setLastError(ESP_ERR_INVALID_STATE, "Falha no HTTP GET: Nao conectado a rede.");

    if (!client.connected())
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao baixar o arquivo: Nao conectado ao servidor.");

    String message = String("GET ") + partialUrl + " HTTP/1.0\r\n";

    client.print(message.c_str());
    ESP_LOGD(TAG_CONNMGR, "%s", message.c_str());
    message = String("Host: ") + g_currentServer.c_str() + "\r\n";
    client.print(message.c_str());
    ESP_LOGD(TAG_CONNMGR, "%s", message.c_str());
    message = "Connection: close\r\n\r\n";
    client.print(message.c_str());
    ESP_LOGD(TAG_CONNMGR, "%s", message.c_str());

    long startTime = millis();
    while (client.available() == 0)
    {
        if ((millis() - startTime) > timeout)
        {
            ESP_LOGE(TAG_CONNMGR, ">>> Client Timeout !");
            return ESP_FAIL;
        }
    }

    ESP_LOGD(TAG_CONNMGR, "Reading response header");
    uint32_t contentLength = 0;

    while (client.available())
    {
        String line = client.readStringUntil('\n');
        line.trim();
        ESP_LOGD(TAG_CONNMGR, "%s", line.c_str()); // Uncomment this to show response header
        line.toLowerCase();

        if (line.startsWith("content-length:"))
        {
            contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
        }
        else if (line.length() == 0)
        {
            break;
        }
    }

    ESP_LOGD(TAG_CONNMGR, "Reading response data");
    startTime = millis();
    uint32_t readLength = 0;

    unsigned long timeElapsed = millis();
    String incoming = "";

    while (readLength < contentLength && client.connected() && (millis() - startTime) < timeout)
    {
        while (client.available())
        {
            uint8_t c = client.read();
            if (readLength < 10240)
                incoming.concat((char)c);

            readLength++;

            if (readLength % (contentLength / 13) == 0)
                ESP_LOGD(TAG_CONNMGR, "Baixado %.2f%%", (100.0 * readLength) / contentLength);

            startTime = millis();
        }
    }

    timeElapsed = millis() - timeElapsed;
    float duration = float(timeElapsed) / 1000;

    ESP_LOGD(TAG_CONNMGR, "Content-Length: %lu", contentLength);
    ESP_LOGD(TAG_CONNMGR, "Actually read: %lu", readLength);
    ESP_LOGD(TAG_CONNMGR, "Duration: %.2fs", duration);
    ESP_LOGD(TAG_CONNMGR, "Content: %s", incoming.c_str());

    *result = incoming;

    return ESP_OK;
}

esp_err_t ConnectionManagerClass::write(uint8_t *buffer, size_t length, size_t *written)
{
    int outputLen = length * 2;
    char output[outputLen + 1];

    Utils.toHexString(buffer, length, output, outputLen + 1);
    ESP_LOGD(TAG_CONNMGR, "Executando write (buffer: %s)", output);

    ConnectionStatus_e status = getConnectionStatus();
    if (status != CONNECTION_STATUS_CONNECTED)
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao escrever o pacote: Nao conectado a rede.");

    if (!client.connected())
        return setLastError(ESP_ERR_INVALID_STATE, "Falha ao escrever o pacote: Nao conectado ao servidor.");

    *written = client.write(buffer, length);

    return ESP_OK;
}

void ConnectionManagerClass::setAutoReconnect(bool autoReconnect)
{
    xSemaphoreTake(m_lock, portMAX_DELAY);
    g_connectionSettings.AutoReconnect = autoReconnect;
    xSemaphoreGive(m_lock);
}

esp_err_t ConnectionManagerClass::power()
{
    gpio_num_t powerPin = GPIO_NUM_12;

    ESP_LOGD("", "Iniciando procedimento de power...");

    // subir o pino POWER por mais de 1 segundo e baixar ela de volta
    pinMode(powerPin, OUTPUT);
    digitalWrite(powerPin, LOW);
    delay(250);

    SerialAT.readString();

    ESP_LOGD("", "[%lu] Subindo nivel da GPIO %d...", millis(), powerPin);
    digitalWrite(powerPin, HIGH);
    delay(1200);

    ESP_LOGD("", "[%lu] Baixando nivel da GPIO %d...", millis(), powerPin);
    digitalWrite(powerPin, LOW);
    delay(2000);

    ESP_LOGD("", "Procedimento de power finalizado...");

    return ESP_OK;
}

ConnectionManagerClass ConnectionManager;