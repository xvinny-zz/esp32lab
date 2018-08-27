/**************************************************************
 *
 * For this example, you need to install PubSubClient library:
 *   https://github.com/knolleary/pubsubclient
 *   or from http://librarymanager/all#PubSubClient
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
 *
 * For more MQTT examples, see PubSubClient library
 *
 **************************************************************
 * Use Mosquitto client tools to work with MQTT
 *   Ubuntu/Linux: sudo apt-get install mosquitto-clients
 *   Windows:      https://mosquitto.org/download/
 *
 * Subscribe for messages:
 *   mosquitto_sub -h test.mosquitto.org -t GsmClientTest/init -t GsmClientTest/ledStatus -q 1
 * Toggle led:
 *   mosquitto_pub -h test.mosquitto.org -t GsmClientTest/led -q 1 -m "toggle"
 *
 * You can use Node-RED for wiring together MQTT-enabled devices
 *   https://nodered.org/
 * Also, take a look at these additional Node-RED modules:
 *   node-red-contrib-blynk-ws
 *   node-red-dashboard
 *
 **************************************************************/
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
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_XBEE

#include <TinyGsmClient.h>
#include <PubSubClient.h>

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

#define GSM_TX 13
#define GSM_RX 14

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[] = "timbrasil.br";
const char user[] = "tim";
const char pass[] = "tim";

// MQTT details
//const char* broker = "test.mosquitto.org";
const char *broker = "m13.cloudmqtt.com";
//const int port = 1883;
const int port = 17690;
const char *mqttUser = "gsrprydz";
const char *mqttPassword = "FyKEyFOBka0U";

const char *topicLed = "GsmClient/led";
const char *topicInit = "GsmClient/init";
const char *topicLedStatus = "GsmClient/ledStatus";

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

#define LED_PIN 13
int ledStatus = LOW;

long lastReconnectAttempt = 0;

void mqttCallback(char *topic, byte *payload, unsigned int len);
boolean mqttConnect();

void setup()
{
    // Set console baud rate
    SerialMon.begin(115200);

    SerialMon.println("Iniciando controlador dos leds...");
    LedController.begin();

    // Set GSM module baud rate
    SerialAT.begin(115200, SERIAL_8N1, GSM_TX, GSM_RX, false);
    delay(3000);

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    SerialMon.println("Inicializando o modem...");
    modem.restart();

    String modemInfo = modem.getModemInfo();
    SerialMon.print("Modem: ");
    SerialMon.println(modemInfo);

    // Unlock your SIM card with a PIN
    //modem.simUnlock("1234");

    SerialMon.print("Waiting for network...");
    if (!modem.waitForNetwork())
    {
        SerialMon.println(" fail");
        while (true)
            ;
    }
    SerialMon.println(" OK");

    while (true)
    {
        SerialMon.printf("Trying to connect to %s...\n", apn);
        if (!modem.gprsConnect(apn, user, pass))
        {
            SerialMon.println(" fail");
            SerialMon.println("Trying again in 1s...");
            delay(1000);
        }
        else
        {
            SerialMon.println(" OK");
            break;
        }
    }

    // MQTT Broker setup
    mqtt.setServer(broker, port);
    mqtt.setCallback(mqttCallback);
}

void loop()
{
    if (!mqtt.connected())
    {
        SerialMon.println("=== MQTT NAO CONECTADO ===");
        // Reconnect every 10 seconds
        unsigned long t = millis();
        if (t - lastReconnectAttempt > 10000L)
        {
            lastReconnectAttempt = t;
            if (mqttConnect())
            {
                lastReconnectAttempt = 0;
            }
        }
        delay(100);
        return;
    }

    mqtt.loop();

    SerialMon.printf("Status GPRS: %s\n", modem.isGprsConnected() ? "connected" : "not connected");
    SerialMon.printf("Qualidade do sinal: %d\n", modem.getSignalQuality());
    SerialMon.printf("Nivel da bateria: %d\n", modem.getBattPercent());
    SerialMon.printf("Tensao da bateria: %.2f\n", modem.getBattVoltage());
    // This is only supported on SIMxxx series
    String gsmTime = modem.getGSMDateTime(DATE_TIME);
    SerialMon.printf("GSM Hora: %s\n", gsmTime.c_str());
    String gsmDate = modem.getGSMDateTime(DATE_DATE);
    SerialMon.printf("GSM Data: %s\n", gsmDate.c_str());
    String cop = modem.getOperator();
    SerialMon.printf("Operadora: %s\n", cop.c_str());
    // String band = modem.getBand();
    // SerialMon.printf("Banda: %s\n", band.c_str());
}

boolean mqttConnect()
{
    SerialMon.print("Connecting to ");
    SerialMon.print(broker);

    // Connect to MQTT Broker
    //boolean status = mqtt.connect("GsmClientTest");

    // Or, if you want to authenticate MQTT:
    boolean status = mqtt.connect("GsmClient", mqttUser, mqttPassword);

    if (status == false)
    {
        SerialMon.println(" fail");
        return false;
    }

    SerialMon.println(" OK");
    mqtt.publish(topicInit, "GSMClient iniciado");
    mqtt.subscribe(topicLed);
    return mqtt.connected();
}

void mqttCallback(char *topic, byte *payload, unsigned int len)
{
    SerialMon.print("Mensagem recebida [");
    SerialMon.print(topic);
    SerialMon.print("]: ");
    SerialMon.write(payload, len);
    SerialMon.println();

    // Only proceed if incoming message's topic matches
    if (String(topic) == topicLed)
    {
        ledStatus = !ledStatus;
        LedController.setLed(LED_ALARME_ESQ_2, ledStatus);
        SerialMon.printf("Publicando status do led: %d\n", ledStatus);
        mqtt.publish(topicLedStatus, ledStatus ? "1" : "0");
    }
}
