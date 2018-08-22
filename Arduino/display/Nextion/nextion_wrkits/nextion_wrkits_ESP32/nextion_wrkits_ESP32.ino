
// =====================================================================
// ---------------------------- Bibliotecas ----------------------------
#include <Nextion.h>
#include <NextionText.h>
#include <NextionProgressBar.h>
#include <NextionDualStateButton.h>
#include <HardwareSerial.h>
#include <StreamDebugger.h>

// =====================================================================
// ---------------------- Mapeamento de Hardware -----------------------
#define   pot   34
#define   led   22
#define   GPIO_NEXTION_TX    13   // ESP_RX
#define   GPIO_NEXTION_RX    17   // ESP_TX

// =====================================================================
// ----------------------- Declaração de objetos -----------------------
HardwareSerial Serial1(2); // RX, TX
Nextion nex(Serial1, false);
NextionProgressBar progressBar(nex, 0, 4, "j0");
NextionText textBoxRandomValue(nex, 0, 2, "t0");
NextionDualStateButton buttonLed(nex, 0, 5, "bt0");

// =====================================================================
// ------------------------- Variáveis globais -------------------------
int randomNumber = 0;    // potenciômetro
int progressBarValue = 0;    // barra de progresso

char txt1[10];        // texto para conversao e exibicao
char txt2[10];

bool isButtonLedActive;      // armazena o estado do botao
int buttonCount;

// =====================================================================
// ---------------------- Configurações iniciais -----------------------

void setup()
{
  Serial.begin( 115200 );
 
  randomSeed( analogRead(0) );
  
  pinMode( led, OUTPUT );                               // define o led como saida
  digitalWrite( led, LOW );                             // o led inicia desligado

  Serial1.begin(9600, SERIAL_8N1, GPIO_NEXTION_TX, GPIO_NEXTION_RX);
  
  while ( !nex.init() )                                         // inicializa o TFT
  {
      delay(2000);
  }

  buttonCount = 0;
}

void loop()
{
    Serial.printf("Setando a progressBar para: %d\n", progressBarValue);
    progressBar.setValue(progressBarValue);
    Serial.printf("Setando o texto para: %s\n", txt1);
    textBoxRandomValue.setText(txt1);

    //textBoxRandomValue = analogRead(pot);
    randomNumber = random(100);
    Serial.printf("Randomico gerado: %d\n", randomNumber);

    memset(txt1, 0, sizeof(txt1));
    itoa(randomNumber, txt1, 10);

    //progressBarValue = map(randomNumber, 0, 1023, 0, 100);
    progressBarValue = randomNumber;

    if(buttonLed.isActive())
        digitalWrite( led, HIGH );
    else
        digitalWrite( led, LOW );

    delay( 1000 );
}
