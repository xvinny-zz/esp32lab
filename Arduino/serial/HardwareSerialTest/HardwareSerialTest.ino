#include <HardwareSerial.h>

HardwareSerial g_HardwareSerial1( 1 );

#define GPS_TX            23
#define GPS_RX            22
#define GPS_BAUD          9600
#define GPS_SERIAL_CONFIG SERIAL_8N1
#define DEBUG_SERIAL_BAUD 115200

void setup() {
  Serial.begin( DEBUG_SERIAL_BAUD );
  g_HardwareSerial1.begin( GPS_BAUD, GPS_SERIAL_CONFIG, GPS_TX, GPS_RX );

  Serial.println("\nSoftware serial test started");

}

void loop() {
  while ( g_HardwareSerial1.available() > 0 ) 
  {
    Serial.write(g_HardwareSerial1.read());
  }
}
