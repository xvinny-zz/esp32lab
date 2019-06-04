/**
 * @file TrackerWrapper.h
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief 
 * @version 0.1
 * @date 29/05/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <string.h>
#include "base64.h"
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>

#define GSM_TX 13
#define GSM_RX 14

#define SerialAT Serial1

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

enum TrackerOperation_e
{
    TRACKER_OP_CONNECT = 0x01,
    TRACKER_OP_DISCONNECT,
    TRACKER_OP_POSITION,
    TRACKER_OP_LOGIN,
    TRACKER_OP_HEARTBEAT,
    TRACKER_OP_GET_DEVICES
};

class TrackerWrapperClass
{
    public:
        TrackerWrapperClass();
        esp_err_t begin();
        esp_err_t queueConnect();
        esp_err_t queueDisconnect();
        esp_err_t queuePosition();
        esp_err_t queueLogin();
        esp_err_t queueHttpGet();
        esp_err_t queueHeartbeat();
        esp_err_t queueGetDevices();
        bool isConnected();
        //void postPosition(String id, double lat, double lng, int batteryLevel);
    private:
        esp_err_t connectNetwork();
};

extern TrackerWrapperClass TrackerWrapper;