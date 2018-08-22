void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  Serial.print("Temperature of the internal sensor: ");
  Serial.print( temperatureRead() );
  Serial.println( "°C" );
  delay(1000);
}
