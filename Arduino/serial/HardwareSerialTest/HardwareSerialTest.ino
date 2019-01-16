#include <HardwareSerial.h>

HardwareSerial g_HardwareSerial( 2 );

#define GPS_TX            17
#define GPS_RX            16
#define GPS_BAUD          115200
#define GPS_SERIAL_CONFIG SERIAL_8N1
#define DEBUG_SERIAL_BAUD 115200

void setup() {
  Serial.begin( DEBUG_SERIAL_BAUD );
  g_HardwareSerial.begin( GPS_BAUD, GPS_SERIAL_CONFIG, GPS_TX, GPS_RX );

  Serial.println("\Hardware serial test started");

}

void loop() {
  while ( g_HardwareSerial.available() > 0 ) 
  {
    Serial.write(g_HardwareSerial.read());
  }
}
