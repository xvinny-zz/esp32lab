#include <Arduino.h>
#include "LedHandler.h"

// Select your modem:
#define TINY_GSM_MODEM_SIM800
// #define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
#include <TinyGsmClient.h>

#define GSM_TX 13
#define GSM_RX 14

#define SerialMon Serial
#define SerialAT Serial1
//#define DUMP_AT_COMMANDS  // uncomment for dump at commands
#define TINY_GSM_DEBUG SerialMon

#define OPERATOR_OI

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
TinyGsm modem(SerialAT);
#endif

//////////////////////////////////
//           PROTÓTIPOS         //
//////////////////////////////////
String gsmWrite(String snd, String rcv);
void testModem();

void setup()
{
    SerialMon.begin(115200);
    LedController.begin();

    SerialMon.println( "Iniciando o teste do GSM..." );
    
    // Set GSM module baud rate
    SerialAT.begin(115200, SERIAL_8N1, GSM_TX, GSM_RX, false);
    delay(1000);
}

void loop()
{

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    SerialMon.println( "Restartando o modem...");

    if (!modem.restart())
    {
        SerialMon.println( "Falha ao restartar o modem.");
        delay(5000);
        return;
    }

    String modemInfo = modem.getModemInfo();
    SerialMon.printf( "Modem: %s\n", modemInfo.c_str());
    SerialMon.println( "Aguardando rede...");
    
    if (!modem.waitForNetwork())
    {
        SerialMon.println( "Rede nao disponivel. Reiniciando...");
        delay(5000);
        return;
    }

    if (modem.isNetworkConnected())
    {
        SerialMon.println( "Rede conectada!" );
    }

    SerialMon.printf( "Conectando ao APN [%s]...\n", apn);
    if (!modem.gprsConnect(apn, user, pass))
    {
        SerialMon.printf( "Falha ao conectar no APN [%s]. Processo de restart em 3s...\n", apn );
        delay(3000);
        return;
    }

    SerialMon.printf( "Status GPRS: %s\n", modem.isGprsConnected() ? "Conectado" : "Nao conectado" );

    String ccid = modem.getSimCCID();
    SerialMon.printf( "CCID: %s\n", ccid.c_str() );

    String imei = modem.getIMEI();
    SerialMon.printf( "IMEI: %s\n", imei.c_str() );

    String cop = modem.getOperator();
    SerialMon.printf( "Operadora: %s\n", cop.c_str() );

    IPAddress local = modem.localIP();
    SerialMon.printf( "IP Local: %s\n", local.toString().c_str());

    SerialMon.printf( "Qualidade sinal: %d\n", modem.getSignalQuality() );

    // This is NOT supported on M590
    SerialMon.printf( "Nivel Bateria: %d\n", (modem.getBattPercent()) );

    // This is only supported on SIMxxx series
    SerialMon.printf( "Tensao Bateria: %.2f\n", (modem.getBattVoltage() / 1000.0F) );

    // This is only supported on SIMxxx series
    String gsmLoc = modem.getGsmLocation();
    SerialMon.printf( "Localizacao GSM: %s\n", gsmLoc.c_str() );

    // This is only supported on SIMxxx series
    String gsmTime = modem.getGSMDateTime(DATE_TIME);
    SerialMon.printf( "GSM Hora: %s\n", gsmTime.c_str() );
    String gsmDate = modem.getGSMDateTime(DATE_DATE);
    SerialMon.printf( "GSM Data: %s\n", gsmDate.c_str() );

    // String ussd_balance = modem.sendUSSD("*111#");
    // SerialMon.printf( "Balance (USSD): %s\n", ussd_balance.c_str() );

    // String ussd_phone_num = modem.sendUSSD( "*161#" );
    // SerialMon.printf( "Phone number (USSD): %s\n", ussd_phone_num.c_str() );

#if defined(TINY_GSM_MODEM_SIM808)
    modem.enableGPS();
    String gps_raw = modem.getGPSraw();
    modem.disableGPS();
    DBG("GPS raw data:", gps_raw);
#endif

#if defined(SMS_TARGET)
    res = modem.sendSMS(SMS_TARGET, String("Hello from ") + imei);
    DBG("SMS:", res ? "OK" : "fail");

    // This is only supported on SIMxxx series
    res = modem.sendSMS_UTF16(SMS_TARGET, u"Привіііт!", 9);
    DBG("UTF16 SMS:", res ? "OK" : "fail");
#endif

#if defined(CALL_TARGET)
    DBG("Calling:", CALL_TARGET);

    // This is NOT supported on M590
    res = modem.callNumber(CALL_TARGET);
    DBG("Call:", res ? "OK" : "fail");

    if (res)
    {
        delay(1000L);

        // Play DTMF A, duration 1000ms
        modem.dtmfSend('A', 1000);

        // Play DTMF 0..4, default duration (100ms)
        for (char tone = '0'; tone <= '4'; tone++)
        {
            modem.dtmfSend(tone);
        }

        delay(5000);

        res = modem.callHangup();
        DBG("Hang up:", res ? "OK" : "fail");
    }
#endif

    modem.gprsDisconnect();
    if (!modem.isGprsConnected())
    {
        ESP_LOGI( TAG_MAIN, "GPRS disconnected" );
    }
    else
    {
        ESP_LOGI( TAG_MAIN, "GPRS disconnect: Failed." );
    }

    // Try to power-off (modem may decide to restart automatically)
    // To turn off modem completely, please use Reset/Enable pins
    modem.poweroff();
    ESP_LOGI( TAG_MAIN, "Poweroff." );

    // Do nothing forevermore
    while (true)
    {
        modem.maintain();
    }
}

void testModem()
{
    String response = gsmWrite("AT", "OK");

    if (response == "FAIL")
        return; //Se o GSM nao responder, retorna a funcao (trava o mcu)
    else
        SerialMon.printf("Resposta: %s", response.c_str());

    delay(500);
}

String gsmWrite(String snd, String rcv) //Funcao que envia dados pro GSM e espera a resposta de ate 30seg
{
    SerialMon.print("Enviando o comando ");
    SerialMon.println(snd);

    SerialAT.println(snd);

    if (rcv.indexOf("+CMGS") > -1)
    {
        delay(150);
        SerialAT.write(0x1A);
    }

    for (uint16_t i = 0; i < 1200; i++)
    {
        delay(25);
        if (SerialAT.available())
        {
            delay(50);
            String a = SerialAT.readString();

            if (a.indexOf(rcv) > -1 || rcv.length() == 0)
            {
                return a;
            }
        }
    }

    return "FAIL";
}