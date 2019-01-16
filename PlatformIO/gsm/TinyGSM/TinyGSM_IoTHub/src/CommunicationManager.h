#ifndef __COMMUNICATION_MANAGER_H__
#define __COMMUNICATION_MANAGER_H__

#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include "esp_log.h"

// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1
#define GSM_TX    13
#define GSM_RX    14

//#define OPERATOR_OI
#define OPERATOR_TIM
//#define OPERATOR_VIVO

typedef struct
{
    bool IsAnyConnected;
    bool IsGPRSConnected;
    bool IsNetworkConnected;
    bool IsModemStarted;
    String IMEI;
    IPAddress LocalIP;
    String Operator;
    int SignalQuality;
    String SimCCID;    
}ConnectionProperties;


typedef struct
{
    char DeviceId[32];
    char Broker[255];
    char User[255];
    char Password[255];
    int Port;
}MQTTSettings_t;

const short SMALL_QUEUE_LENGTH = 10;
const short SMALL_QUEUE_ITEM_SIZE = 512;
const short BIG_QUEUE_LENGTH = 10;
const short BIG_QUEUE_ITEM_SIZE = 2048;


class CommunicationManagerClass
{
    public:
        CommunicationManagerClass(){}
        bool isConnected();
        esp_err_t begin( MQTTSettings_t settings );
        esp_err_t sendMessage( char * message , size_t size );
    private:
        esp_err_t initializeMqtt();
        QueueHandle_t m_smallMessageQueue;
        QueueHandle_t m_bigMessageQueue;
};

extern CommunicationManagerClass CommunicationManager;

#endif // __COMMUNICATION_MANAGER_H__