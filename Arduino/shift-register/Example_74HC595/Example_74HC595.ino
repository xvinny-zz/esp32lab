#define PIN_DATA           22    // SER
#define PIN_LOAD           21    // RCLK
#define PIN_CLOCK          4     // SRCLK

#define LEDC_CHANNEL_0        0
#define LEDC_TIMER_RESOLUTION 8
#define LEDC_BASE_FREQ        10000
#define LED_PIN               15

byte ledChannel = LEDC_CHANNEL_0;

void setup() 
{
  pinMode( PIN_DATA  , OUTPUT );
  pinMode( PIN_LOAD  , OUTPUT );
  pinMode( PIN_CLOCK , OUTPUT );
   // Setup timer and attach timer to a led pin
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_RESOLUTION);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
}

void loop() 
{
  byte data = 1;

  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , data );
  digitalWrite( PIN_LOAD , HIGH );
    
  for ( int dutyCycle = 0 ; dutyCycle <= 255 ; dutyCycle++ )
  {
    ledcWrite( ledChannel, dutyCycle );  
    delay(7);
  }
  
  delay( 1000 );

  data = 2;
  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , data );
  digitalWrite( PIN_LOAD , HIGH );
  for ( int dutyCycle = 0 ; dutyCycle <= 255 ; dutyCycle++ )
  {
    ledcWrite( ledChannel, dutyCycle );  
    delay(7);
  }
  
  delay( 1000 );

  data = 3;
  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , data );
  digitalWrite( PIN_LOAD , HIGH );
  for ( int dutyCycle = 0 ; dutyCycle <= 255 ; dutyCycle++ )
  {
    ledcWrite( ledChannel, dutyCycle );  
    delay(7);
  }
  
  delay( 1000 );

  data = 4;
  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , data );
  digitalWrite( PIN_LOAD , HIGH );
  for ( int dutyCycle = 0 ; dutyCycle <= 255 ; dutyCycle++ )
  {
    ledcWrite( ledChannel, dutyCycle );  
    delay(7);
  }
  
  delay( 1000 );

  data = 8;
  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , data );
  digitalWrite( PIN_LOAD , HIGH );
  for ( int dutyCycle = 0 ; dutyCycle <= 255 ; dutyCycle++ )
  {
    ledcWrite( ledChannel, dutyCycle );  
    delay(7);
  }
  
  delay( 1000 );

  data = 16;
  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , data );
  digitalWrite( PIN_LOAD , HIGH );
  for ( int dutyCycle = 0 ; dutyCycle <= 255 ; dutyCycle++ )
  {
    ledcWrite( ledChannel, dutyCycle );  
    delay(7);
  }
  
  delay( 1000 );

  data = 32;
  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , data );
  digitalWrite( PIN_LOAD , HIGH );
  for ( int dutyCycle = 0 ; dutyCycle <= 255 ; dutyCycle++ )
  {
    ledcWrite( ledChannel, dutyCycle );  
    delay(7);
  }
  
  delay( 1000 );

  data = 64;
  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , data );
  digitalWrite( PIN_LOAD , HIGH );
  for ( int dutyCycle = 0 ; dutyCycle <= 255 ; dutyCycle++ )
  {
    ledcWrite( ledChannel, dutyCycle );  
    delay(7);
  }
  
  delay( 1000 );

  data = 128;
  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , data );
  digitalWrite( PIN_LOAD , HIGH );
  for ( int dutyCycle = 0 ; dutyCycle <= 255 ; dutyCycle++ )
  {
    ledcWrite( ledChannel, dutyCycle );  
    delay(7);
  }
  
  delay( 1000 );

  data = 0;
  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , data );
  digitalWrite( PIN_LOAD , HIGH );
  for ( int dutyCycle = 0 ; dutyCycle <= 255 ; dutyCycle++ )
  {
    ledcWrite( ledChannel, dutyCycle );  
    delay(7);
  }
  
  delay( 1000 );  
}
