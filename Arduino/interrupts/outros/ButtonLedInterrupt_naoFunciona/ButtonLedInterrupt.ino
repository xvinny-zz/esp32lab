
byte buttonAuto = 34;
byte buttonSpeed = 35;
byte ledAuto = 32;
byte ledSpeed = 33;
volatile byte ledAutoState = LOW;
volatile byte ledSpeedState = LOW;
unsigned long debounceDelay = 200;    // the debounce time; increase if the output flickers
unsigned long lastDebounceTimeLedAuto = 0;
unsigned long lastDebounceTimeLedSpeed = 0;

void setup() 
{
  Serial.begin(115200);
  Serial.println(F("Iniciando teste da interrupção..."));
  
  pinMode( ledAuto , OUTPUT );
  pinMode( ledSpeed , OUTPUT );

  pinMode( buttonAuto , INPUT_PULLUP );
  pinMode( buttonSpeed , INPUT_PULLUP );
  
  attachInterrupt( digitalPinToInterrupt ( buttonAuto ) , blinkAuto , FALLING );
  attachInterrupt( digitalPinToInterrupt ( buttonSpeed ) , blinkSpeed , FALLING );
}

void loop() 
{
}

void blinkAuto()
{
  Serial.println(F("AUTO Pressionado!"));
  ledAutoState = !ledAutoState;
  digitalWrite( ledAuto, ledAutoState );
  delay(200);
}

void blinkSpeed()
{
  Serial.println(F("VELOCIDADE Pressionado!"));
  ledSpeedState = !ledSpeedState;
  digitalWrite( ledSpeed, ledSpeedState );
  delay(200);
}

