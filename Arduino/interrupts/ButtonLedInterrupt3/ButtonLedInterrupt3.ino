// Pino do led do modo automático.
#define LED_AUTO 32
// Pino do led de velocidade.
#define LED_SPEED 33
// Pino do botão do modo automático.
#define BUTTON_AUTO 34
// Pino do botão de velocidade.
#define BUTTON_SPEED 35
// Tempo de espera do debounce dos eventos dos botões, em milissegundos.
#define DEBOUNCE_WAIT_TIME 200

/* Variáveis */
// Armazena o estado do LED_AUTO
volatile byte LED_AUTO_state = LOW;
// Armazena o estado do LED_SPEED
volatile byte LED_SPEED_state = LOW;  
// Armazena o horário da última interrupção do BUTTON_AUTO
static unsigned long BUTTON_AUTO_lastInterruptTime = 0;
// Armazena o horário da interrupção atual do BUTTON_AUTO
unsigned long BUTTON_AUTO_interruptTime = 0;
// Armazena o horário da última interrupção do BUTTON_SPEED
static unsigned long BUTTON_SPEED_lastInterruptTime = 0;
// Armazena o horário da interrupção atual do BUTTON_SPEED
unsigned long BUTTON_SPEED_interruptTime = 0;

void setup() 
{
  /* Configura os pinos dos LEDs como OUTPUT*/
  pinMode(LED_AUTO, OUTPUT);
  pinMode(LED_SPEED, OUTPUT);
  
  /* Configura os pinos dos botões como INPUT_PULLUP*/
  pinMode(BUTTON_AUTO, INPUT_PULLUP);
  pinMode(BUTTON_SPEED, INPUT_PULLUP);
  
  /* Relaciona as interrupções aos pinos dos botões.
    As respectivas funções blink vão ser invocadas quando a interrupção ocorrer.
    A interrupção ocorre somente na borda de descida (FALLING). */
  attachInterrupt(digitalPinToInterrupt(BUTTON_AUTO), blinkAuto, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_SPEED), blinkSpeed, FALLING);
}

void loop()
{
}

/* interrupt function toggle the LED */
void blinkAuto()
{
  BUTTON_AUTO_interruptTime = millis();

  if (BUTTON_AUTO_interruptTime - BUTTON_AUTO_lastInterruptTime > DEBOUNCE_WAIT_TIME) 
  {
    LED_AUTO_state = !LED_AUTO_state;
    digitalWrite(LED_AUTO, LED_AUTO_state);
  }

  BUTTON_AUTO_lastInterruptTime = BUTTON_AUTO_interruptTime;
}

void blinkSpeed()
{
  BUTTON_SPEED_interruptTime = millis();

  if (BUTTON_SPEED_interruptTime - BUTTON_SPEED_lastInterruptTime > DEBOUNCE_WAIT_TIME) 
  {
    LED_SPEED_state = !LED_SPEED_state;
    digitalWrite(LED_SPEED, LED_SPEED_state);
  }

  BUTTON_SPEED_lastInterruptTime = BUTTON_SPEED_interruptTime;
}
