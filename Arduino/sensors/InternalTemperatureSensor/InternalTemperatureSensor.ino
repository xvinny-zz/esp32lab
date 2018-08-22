#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif

uint8_t temprature_sens_read();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  uint8_t fahrenheit = 0;
  
  // put your main code here, to run repeatedly:
  Serial.print("Temperature of the internal sensor: ");
  fahrenheit = temprature_sens_read();
  Serial.print( fahrenheit );
  Serial.print( "°F / " );
  //Serial.print( fahrenheitToCelsius( fahrenheit ));
  Serial.print( temperatureRead() );
  Serial.println( "°C" );
  delay(1000);
}

float fahrenheitToCelsius(uint8_t temperature)
{
  return ((temperature - 32 ) / 1.8);
}

