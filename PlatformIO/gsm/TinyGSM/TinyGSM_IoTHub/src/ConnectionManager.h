#ifndef __CONNECTION_MANAGER_H__
#define __CONNECTION_MANAGER_H__

#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>

// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1
#define GSM_TX    13
#define GSM_RX    14

#define OPERATOR_OI
//#define OPERATOR_TIM

struct ConnectionProperties
{
    bool IsGPRSConnected;
    bool IsNetworkConnected;
    bool IsModemStarted;
    String IMEI;
    IPAddress LocalIP;
    String Operator;
    int SignalQuality;
    String SimCCID;    
};


class ConnectionManagerClass
{
    public:
        ConnectionManagerClass();
        void begin();
        void postMessage();
};

extern ConnectionManagerClass ConnectionManager;

#endif // __CONNECTION_MANAGER_H__