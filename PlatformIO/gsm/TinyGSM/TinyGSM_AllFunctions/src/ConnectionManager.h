/**
 * @file ConnectionManager.h
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief 
 * @version 0.1
 * @date 21/02/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#pragma once

////////////////////////////////////////////////////////////////////////
//                          INCLUDES
////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <SPI.h>
#include "SdFat.h"
#include "Utils.h"

////////////////////////////////////////////////////////////////////////
//                          DEFINES
////////////////////////////////////////////////////////////////////////
// Descomente para debugar o GSM
//#define DEBUG_GSM

#define GSM_TX 13
#define GSM_RX 14

#define SerialAT Serial1

#define TAG_CONNMGR "[CONNMGR]"
#define CONNMGR_LOGD_0001 "[begin] (APN:%s,User:%s,Password:%s)"

#define CON_ERR_0001        0x100
#define CON_ERR_0002        0x101
#define CON_ERR_0003        0x102
#define CON_ERR_0004        0x103
#define CON_ERR_MSG_0001    "Falha ao restartar o modem"
#define CON_ERR_MSG_0002    "Rede nao disponivel"
#define CON_ERR_MSG_0003    "Nao conectado a rede"
#define CON_ERR_MSG_0004    "Timeout de comunicacao no power on"



////////////////////////////////////////////////////////////////////////
//                          ENUMS
////////////////////////////////////////////////////////////////////////
/**
 * @brief Status de conectividade.
 * 
 */
enum ConnectionStatus_e
{
    /**
     * @brief Desconectando
     */
    CONNECTION_STATUS_DISCONNECTING = 0x00,
    /**
     * @brief Desconectado
     */
    CONNECTION_STATUS_DISCONNECTED,
    /**
     * @brief Conectando
     */
    CONNECTION_STATUS_CONNECTING,
    /**
     * @brief Conectado
     */
    CONNECTION_STATUS_CONNECTED
};

/**
 * @brief Operações assíncronas de conexão.
 * 
 */
enum AsyncConnectionOperation_e
{
    /**
     * @brief Conectar o GSM.
     * 
     */
    CON_OP_CONNECT = 0x01,
    CON_OP_CONNECT_CLIENT = 0x02
};

////////////////////////////////////////////////////////////////////////
//                          STRUCTS
////////////////////////////////////////////////////////////////////////
/**
 * @brief Detalhes do erro de conexão.
 * 
 */
struct ConnectionError_t
{
    /**
     * @brief Mensagem de erro.
     * 
     */
    char Message[256];
    /**
     * @brief Código do erro.
     * 
     */
    int Code;
};

/**
 * @brief Configurações de conexão da operadora.
 * 
 */
struct ConnectionSettings_t
{
    /**
     * @brief APN da operadora.
     */
    char APN[128];
    /**
     * @brief Usuário da operadora.
     */
    char User[64];
    /**
     * @brief Senha da operadora.
     */
    char Password[64];
    /**
     * @brief Flag que define se é para reconectar automaticamente.
     */
    bool AutoReconnect;
};

/**
 * @brief Dados da operaçao.
 * 
 */
struct AsyncOperationData_t
{
    /**
     * @brief Operação de conexão.
     * 
     */
    AsyncConnectionOperation_e Operation;
    /**
     * @brief Parâmetros (string delimitada por ;).
     * 
     */
    char Parameters[64];
    /**
     * @brief Timestamp da operação.
     * 
     */
    time_t Time;
};

////////////////////////////////////////////////////////////////////////
//                          DELEGATES
////////////////////////////////////////////////////////////////////////
typedef void (*ConnectionEventHandler)(esp_err_t);
typedef void (*ConnectClientEventHandler)(esp_err_t);

////////////////////////////////////////////////////////////////////////
//                          CLASSE
////////////////////////////////////////////////////////////////////////

/**
 * @brief Gerencia a conexão com a rede.
 * 
 */
class ConnectionManagerClass
{
    public:
        /**
         * @brief Inicializar o objeto ConnectionManager.
         * 
         */
        ConnectionManagerClass();
        
        /**
         * @brief Inicializar o ConnectionManager.
         * 
         * @param settings Configurações de rede.
         */
        esp_err_t begin(ConnectionSettings_t settings);
        
        /**
         * @brief Conectar na rede de forma assíncrona.
         * 
         */
        esp_err_t connectAsync(ConnectionEventHandler handler, bool autoReconnect = false);        

        /**
         * @brief Desconectar da rede.
         * 
         */
        void disconnect();

        /**
         * @brief Retornar o IP do dispositivo.
         * 
         * @return String Endereço IP do dispositivo.
         */
        String getIP();
        
        /**
         * @brief Retornar o último erro ocorrido.
         * 
         * @return ConnectionError_t Último erro ocorrido.
         */
        ConnectionError_t getLastError();

        /**
         * @brief Imprimir o último erro ocorrido na serial.
         * 
         */
        void printLastError();

        /**
         * @brief Retornar o status de conectividade atual.
         * 
         * @return ConnectionStatus_e Status de conectividade.
         */
        ConnectionStatus_e getStatus();

        /**
         * @brief Retornar o CCID do device.
         * 
         * @param result 
         * @return esp_err_t 
         */
        esp_err_t getSimCCID(String *result);

        /**
         * @brief Retornar o IMEI.
         * 
         * @param result 
         * @return esp_err_t 
         */
        esp_err_t getIMEI(String *result);

        /**
         * @brief Retornar a operadora.
         * 
         * @param result Operadora.
         * @return esp_err_t Resultado da operação.
         */
        esp_err_t getOperator(String *result);

        /**
         * @brief Retornar o IP local.
         * 
         * @param result IP local.
         * @return esp_err_t Resultado da operação.
         */
        esp_err_t getLocalIP(String *result);
        /**
         * @brief Retornar o nivel de sinal.
         * 
         * @param result Nivel de sinal.
         * @return esp_err_t Resultado da operação.
         */
        esp_err_t getSignalQuality(int16_t *result);
        /**
         * @brief Retornar o nivel de bateria
         * 
         * @param result Nivel de bateria, em %.
         * @return esp_err_t Resultado da operação.
         */
        esp_err_t getBatteryLevel(int8_t *result);
        /**
         * @brief Retornar a tensão da bateria
         * 
         * @param result Tensão bateria, em V.
         * @return esp_err_t Resultado da operação.
         */
        esp_err_t getBatteryVoltage(double *result);
        /**
         * @brief Retornar a localização por GSM.
         * 
         * @param result Localização.
         * @return esp_err_t Resultado da operação.
         */
        esp_err_t getGsmLocation(String *result);
        /**
         * @brief Retornar a data e hora da rede GSM.
         * 
         * @param result Data e hora.
         * @return esp_err_t Resultado da operação.
         */
        esp_err_t getDateTime(String *result);
        /**
         * @brief Enviar uma SMS.
         * 
         * @param number Numero que a SMS sera enviada.
         * @param text Mensagem a ser enviada.
         * @return esp_err_t Resultado da operacao.
         */
        esp_err_t sendSMS(String number, String text);
        esp_err_t connectClient(const char * server, int port, int timeout_s);
        esp_err_t connectClientAsync(ConnectClientEventHandler handler, const char * server, int port, int timeout_s);
        esp_err_t disconnectClient();
        esp_err_t downloadFile(const char * resource, uint32_t fileSize, const char * destinationFile, time_t timeout);
        esp_err_t httpGet(const char * partialUrl, String * result, int timeout);
        esp_err_t httpsGet(const char * url, const char * certificate, String * result, int timeout);
        esp_err_t write(uint8_t * buffer, size_t length, size_t *written);
        bool isClientConnected();
        void setAutoReconnect(bool autoReconnect);
        esp_err_t power();
    private:
        bool m_isStarted;
};

/**
 * @brief Objeto ConnectionManager exportado.
 * 
 */
extern ConnectionManagerClass ConnectionManager;