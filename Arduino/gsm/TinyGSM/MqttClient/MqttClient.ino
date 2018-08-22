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

#define GSM_TX    13
#define GSM_RX    14
#define PIN_CLOCK   4     // Pin connected to SH_CP (11) of 74HC595 - SRCLK
#define PIN_OE      27    // Pin connected to OE (13)    of 74HC595 - OE
#define PIN_DATA    15    // Pin connected to DS (14)    of 74HC595 - SER
#define PIN_LOAD    21    // Pin connected to ST_CP (10) of 74HC595 - RCLK

HardwareSerial Serial1(1);

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX


// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "timbrasil.br";
const char user[] = "tim";
const char pass[] = "tim";

// MQTT details
//const char* broker = "test.mosquitto.org";
const char* broker = "m13.cloudmqtt.com";
//const int port = 1883;
const int port = 17690;
const char * mqttUser = "gsrprydz";
const char * mqttPassword = "FyKEyFOBka0U";

const char* topicLed = "GsmClient/led";
const char* topicInit = "GsmClient/init";
const char* topicLedStatus = "GsmClient/ledStatus";

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

#define LED_PIN 13
int ledStatus = LOW;

long lastReconnectAttempt = 0;

void setup() 
{
    //pinMode(LED_PIN, OUTPUT);
  
    // Set console baud rate
    SerialMon.begin(115200);
    delay(10);

    SerialMon.println("Turning leds off...");
    pinMode( PIN_LOAD, OUTPUT );
    pinMode( PIN_OE, OUTPUT );
    digitalWrite( PIN_OE, LOW ); 
  
    digitalWrite( PIN_LOAD, 0 );
    LedHandler_ShiftOut( PIN_DATA, PIN_CLOCK, 0 );
    digitalWrite( PIN_LOAD, 1 );
  
    // Set GSM module baud rate
    SerialAT.begin(115200, SERIAL_8N1, GSM_TX , GSM_RX, false);
    delay(3000);
  
    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    SerialMon.println("Initializing modem...");
    modem.restart();
  
    String modemInfo = modem.getModemInfo();
    SerialMon.print("Modem: ");
    SerialMon.println(modemInfo);
  
    // Unlock your SIM card with a PIN
    //modem.simUnlock("1234");
  
    SerialMon.print("Waiting for network...");
    if (!modem.waitForNetwork()) {
      SerialMon.println(" fail");
      while (true);
    }
    SerialMon.println(" OK");

    while( true )
    {
        SerialMon.printf("Trying to connect to %s...\n", apn);
        if ( !modem.gprsConnect( apn, user, pass ) )
        {
            SerialMon.println( " fail" );
            SerialMon.println( "Trying again in 1s..." );
            delay( 1000 );
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

boolean mqttConnect() {
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
  mqtt.publish(topicInit, "GsmClient started");
  mqtt.subscribe(topicLed);
  return mqtt.connected();
}

void loop() 
{
  if (!mqtt.connected()) 
  {
      SerialMon.println("=== MQTT NOT CONNECTED ===");
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

  SerialMon.printf("GPRS status: %s\n", modem.isGprsConnected() ? "connected" : "not connected");
  SerialMon.printf("Signal quality: %d\n", modem.getSignalQuality());
  SerialMon.printf("Battery lavel: %d\n", modem.getBattPercent());
  SerialMon.printf("Battery voltage: %.2f\n", modem.getBattVoltage());
  // This is only supported on SIMxxx series
  String gsmTime = modem.getGSMDateTime(DATE_TIME);
  SerialMon.printf("GSM Time: %s\n", gsmTime.c_str());
  String gsmDate = modem.getGSMDateTime(DATE_DATE);
  SerialMon.printf("GSM Date: %s\n", gsmDate.c_str());
  String cop = modem.getOperator();
  SerialMon.printf("Operator: %s\n", cop.c_str());
  String band = modem.getBand();
  SerialMon.printf("Band: %s\n", band.c_str());
}

void mqttCallback(char* topic, byte* payload, unsigned int len) 
{
    SerialMon.print("Message arrived [");
    SerialMon.print(topic);
    SerialMon.print("]: ");
    SerialMon.write(payload, len);
    SerialMon.println();
    
    // Only proceed if incoming message's topic matches
    if (String(topic) == topicLed) 
    {
        ledStatus = !ledStatus;
        // digitalWrite(LED_PIN, ledStatus);
        SerialMon.printf("Publishing led status: %d\n", ledStatus);
        mqtt.publish(topicLedStatus, ledStatus ? "1" : "0");
    }
}

void LedHandler_ShiftOut( int myDataPin, int myClockPin, byte myDataOut ) 
{
    // This shifts 8 bits out MSB first, on the rising edge of the clock, clock idles low
    
    //internal function setup
    int i = 0;
    int pinState;
    pinMode( myClockPin, OUTPUT );
    pinMode( myDataPin, OUTPUT );
    
    //clear everything out just in case to prepare shift register for bit shifting
    digitalWrite( myDataPin, 0 );
    digitalWrite( myClockPin, 0 );
    
    //for each bit in the byte myDataOut
    //NOTICE THAT WE ARE COUNTING DOWN in our for loop
    //This means that %00000001 or "1" will go through such
    //that it will be pin Q0 that lights. 
    for (i = 7; i >= 0; i-- )  
    {
        digitalWrite(myClockPin, 0);
    
        //if the value passed to myDataOut and a bitmask result 
        // true then... so if we are at i=6 and our value is
        // %11010100 it would the code compares it to %01000000 
        // and proceeds to set pinState to 1.
        if ( myDataOut & (1<<i) ) 
        {
            pinState= 1;
        }
        else 
        {  
            pinState= 0;
        }
    
        //Sets the pin to HIGH or LOW depending on pinState
        digitalWrite( myDataPin, pinState );
        //register shifts bits on upstroke of clock pin  
        digitalWrite( myClockPin, 1 );
        //zero the data pin after shift to prevent bleed through
        digitalWrite( myDataPin, 0 );
    }
    
    //stop shifting
    digitalWrite( myClockPin, 0 );
}
