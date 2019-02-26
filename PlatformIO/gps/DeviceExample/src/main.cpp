#include <Arduino.h>
#include <TinyGPS++.h>

#define SerialGPS Serial2
#define SERIAL_BAUD_RATE 115200
#define GPS_BAUD_RATE 115200

static void smartDelay(unsigned long ms);

TinyGPSPlus gps;

/**
 * Pino TX do GPS ligado na GPIO 16.
 */
const gpio_num_t PIN_GPS_TX = GPIO_NUM_16; // TX do GPS
/**
 * Pino RX do GPS ligado na GPIO 17.
 */
const gpio_num_t PIN_GPS_RX = GPIO_NUM_17; // RX do GPS

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
    SerialGPS.begin(GPS_BAUD_RATE, SERIAL_8N1, (int8_t)PIN_GPS_TX, (int8_t)PIN_GPS_RX);
}

void loop()
{
    Serial.printf("Baud rate : %d\n", GPS_BAUD_RATE);
    Serial.print("Latitude  : ");
    Serial.println(gps.location.lat(), 5);
    Serial.print("Longitude : ");
    Serial.println(gps.location.lng(), 4);
    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());
    Serial.print("Altitude  : ");
    Serial.print(gps.altitude.feet() / 3.2808);
    Serial.println("M");
    Serial.print("Time      : ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.println(gps.time.second());
    Serial.println("**********************");

    smartDelay(1000);

    if (millis() > 5000 && gps.charsProcessed() < 10)
        Serial.println(F("No GPS data received: check wiring"));
}

static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    int incomingByte = 0;

    do
    {
        while (SerialGPS.available())
        {
            incomingByte = SerialGPS.read();
            gps.encode(incomingByte);
            Serial.print((char)incomingByte);
        }
            
    } while (millis() - start < ms);

    Serial.println();
}