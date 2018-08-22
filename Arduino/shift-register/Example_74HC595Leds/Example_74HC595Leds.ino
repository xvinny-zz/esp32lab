
#define PIN_CLOCK   4     // Pin connected to SH_CP of 74HC595 SRCLK
#define LED_PIN     27    // Pin OE
#define PIN_DATA    15    // Pin connected to DS of 74HC595
#define PIN_LOAD     21    // Pin connected to ST_CP of 74HC595


//Define os pinos de saida

int OE    = 27;   // 13 - OE
int SER   = 15;  // 14 - DS
int RCLK  = 21;  // 12 - ST_CP
int SRCLK = 4;   // 11 - SH_CP

//array dos pinos, iniciando em LOW
int regis[] = {0,0,0,0,0,0,0,0};
//contador para fazer acender os LEDs em sequencia
int counter = 0;
int sequence[] = {1,2,4,3,5,6,8,7};
 
//função para efetivar a modificação após mudança dos pinos
void changeValues(int val)
{
    //clear();
    if (val < 8)
    {
        regis[val] = 1;
    }
    digitalWrite(RCLK, LOW);
    
    for(int i=7; i >= 0; i--)
    {
        digitalWrite(SRCLK, LOW);
        int PIN = regis[i];
        digitalWrite(SER, PIN);
        digitalWrite(SRCLK, HIGH);
    }
    digitalWrite(RCLK, HIGH);
}
 
//Baixa todos os pinos do 74HC595
void clear()
{
    for(int i = 7; i >= 0; i--)
    {
        regis[i] = LOW;
    }
} 
 
 
void setup()
{
    Serial.begin(115200);

    //configurando pinos como saida
    Serial.println("configurando pinos como saida");
    pinMode(SER,OUTPUT);
    pinMode(RCLK,OUTPUT);
    pinMode(SRCLK,OUTPUT);
    pinMode(OE,OUTPUT);
    
    digitalWrite(OE, LOW); 
    //Parametro 9 indica que deve zerar os pinos do 74HC595
    Serial.println("zerando os pinos do 74HC595");
    changeValues(9);
}
 
void loop()
{
    Serial.printf("Counter: %d\n", counter);
    Serial.printf("Sequence[counter]: %d\n", sequence[counter]);
    //changeValues(counter);
    changeValues( sequence[counter] );
    Serial.println("Valor alterado! Aguardando...");
    counter += 1;

    //Apos aplicar a mudança, faz um delay
    delay(4000);
    if (counter > 7)
    {
        counter = 0;
    }
}
