#include <TinyGPS++.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
HardwareSerial Serial1(2);

void setup()
{
  Serial.begin( 115200 );
  Serial1.begin( 9600, SERIAL_8N1, 16, 17 );

  Serial.println( F( "DeviceExample" ) );
  Serial.println( F( "Exemplo de uso do TinyGPS++ com um módulo GPS" ) );
  Serial.print( F( "Testando biblioteca TinyGPS++ v. " ) ); 
  Serial.println( TinyGPSPlus::libraryVersion() );
  Serial.println();
}

void loop()
{
  Serial.print( "Latitude  : " );
  Serial.println( gps.location.lat(), 5 );
  Serial.print( "Longitude : " );
  Serial.println( gps.location.lng(), 4 );
  Serial.print( "Satellites: " );
  Serial.println( gps.satellites.value() );
  Serial.print( "Altitude  : " );
  Serial.print( gps.altitude.feet() / 3.2808 );
  Serial.println( "M");
  Serial.print( "Time      : " );
  Serial.print( gps.time.hour() );
  Serial.print( ":");
  Serial.print( gps.time.minute() );
  Serial.print( ":");
  Serial.println( gps.time.second() );
  Serial.println( "**********************" );

  smartDelay( 1000 );  

  if ( millis() > 5000 && gps.charsProcessed() < 10 )
    Serial.println( F( "No GPS data received: check wiring" ) );
}

static void smartDelay(unsigned long ms)                
{
  unsigned long start = millis();
  
  do
  {
    while ( Serial1.available() )
      gps.encode( Serial1.read() );
  } while ( millis() - start < ms );
}
