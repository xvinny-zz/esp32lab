/**************************************************************
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
 *
 * NOTE:
 * Some of the functions may be unavailable for your modem.
 * Just comment them out.
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

#define GSM_TX    13
#define GSM_RX    14
#define PIN_CLOCK   4     // Pin connected to SH_CP (11) of 74HC595 - SRCLK
#define PIN_OE      27    // Pin connected to OE (13)    of 74HC595 - OE
#define PIN_DATA    15    // Pin connected to DS (14)    of 74HC595 - SER
#define PIN_LOAD    21    // Pin connected to ST_CP (10) of 74HC595 - RCLK

// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

HardwareSerial Serial1(1);

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX


//#define DUMP_AT_COMMANDS
#define TINY_GSM_DEBUG SerialMon


// Set phone numbers, if you want to test SMS and Calls
//#define SMS_TARGET  "+380xxxxxxxxx"
//#define CALL_TARGET "+380xxxxxxxxx"

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "timbrasil.br";
const char user[] = "tim";
const char pass[] = "tim";

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);

  SerialMon.println("Iniciando o teste do GSM");
  // Set your reset, enable, power pins here

  delay(500);

  pinMode( PIN_LOAD, OUTPUT );
  pinMode( PIN_OE, OUTPUT );
  digitalWrite( PIN_OE, LOW ); 

  digitalWrite( PIN_LOAD, 0 );
  LedHandler_ShiftOut( PIN_DATA, PIN_CLOCK, 0 );
  digitalWrite( PIN_LOAD, 1 );

  // Set GSM module baud rate
  SerialAT.begin(115200, SERIAL_8N1, GSM_TX , GSM_RX, false);
  delay(1000);

  // Set GSM module baud rate
  //TinyGsmAutoBaud(SerialAT);
  //for ( int i = 0 ; i < 10 ; i++)
  //  TestModem();
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

void TestModem()
{
   String response = gsmWrite("AT", "OK");
    
    if ( response == "FAIL" )
        return;//Se o GSM nao responder, retorna a funcao (trava o mcu)
    else
        SerialMon.printf("Resposta: %s", response.c_str() );
    
    delay( 500 );
}

String gsmWrite(String snd, String rcv)//Funcao que envia dados pro GSM e espera a resposta de ate 30seg
{
    SerialMon.print("Enviando o comando ");
    SerialMon.println( snd );
    
    SerialAT.println( snd );

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

void loop() {

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  if (!modem.restart()) 
  {
    SerialMon.println("Falha ao restartar o modem.");
    delay(10000);
    return;
  }

  String modemInfo = modem.getModemInfo();
  SerialMon.printf("Modem: %s\n", modemInfo.c_str());

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");

  SerialMon.println("Waiting for network...");
  if (!modem.waitForNetwork()) 
  {
      SerialMon.println("Network not available");
      delay(10000);
      return;
  }

  if (modem.isNetworkConnected()) 
  {
      SerialMon.println("Network connected");
  }

  SerialMon.printf("Connecting to %s\n", apn);
  if (!modem.gprsConnect(apn, user, pass)) 
  {
      SerialMon.printf("Failed connecting to %s. Restarting process in 3s...\n", apn);
      delay(3000);
      return;
  }

  bool res = modem.isGprsConnected();
  SerialMon.printf("GPRS status: %s\n", res ? "connected" : "not connected");

  String ccid = modem.getSimCCID();
  SerialMon.printf("CCID: %s\n", ccid.c_str());

  String imei = modem.getIMEI();
  SerialMon.printf("IMEI: %s\n", imei.c_str());

  String cop = modem.getOperator();
  SerialMon.printf("Operator: %s\n", cop);
  
  IPAddress local = modem.localIP();
  SerialMon.print("Local IP: ");
  SerialMon.println( local );

  int csq = modem.getSignalQuality();
  SerialMon.printf("Signal quality: $d\n", csq);

  // This is NOT supported on M590
  int battLevel = modem.getBattPercent();
  SerialMon.printf("Battery lavel: %d\n", battLevel);

  // This is only supported on SIMxxx series
  float battVoltage = modem.getBattVoltage() / 1000.0F;
  SerialMon.printf("Battery voltage: %.2f\n", battVoltage);

  // This is only supported on SIMxxx series
  String gsmLoc = modem.getGsmLocation();
  SerialMon.printf("GSM location: %s\n", gsmLoc.c_str());

  // This is only supported on SIMxxx series
  String gsmTime = modem.getGSMDateTime(DATE_TIME);
  SerialMon.printf("GSM Time: %s\n", gsmTime.c_str());
  String gsmDate = modem.getGSMDateTime(DATE_DATE);
  SerialMon.printf("GSM Date: %s\n", gsmDate.c_str());

  String ussd_balance = modem.sendUSSD("*111#");
  SerialMon.printf("Balance (USSD): %s", ussd_balance.c_str());

  String ussd_phone_num = modem.sendUSSD("*161#");
  SerialMon.printf("Phone number (USSD): %s\n", ussd_phone_num.c_str());

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

  if (res) {
    delay(1000L);

    // Play DTMF A, duration 1000ms
    modem.dtmfSend('A', 1000);

    // Play DTMF 0..4, default duration (100ms)
    for (char tone='0'; tone<='4'; tone++) {
      modem.dtmfSend(tone);
    }

    delay(5000);

    res = modem.callHangup();
    DBG("Hang up:", res ? "OK" : "fail");
  }
#endif

  modem.gprsDisconnect();
  if (!modem.isGprsConnected()) {
    SerialMon.println("GPRS disconnected");
  } else {
    SerialMon.println("GPRS disconnect: Failed.");
  }

  // Try to power-off (modem may decide to restart automatically)
  // To turn off modem completely, please use Reset/Enable pins
  modem.poweroff();
  SerialMon.println("Poweroff.");

  // Do nothing forevermore
  while (true) {
    modem.maintain();
  }
}

