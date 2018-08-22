#define PIN_COM         0    // COM
#define PIN_S0          2      // S0
#define PIN_S1          27     // S1
#define PIN_S2          15     // S2
#define PIN_S3          21     // S3

const int channel[] = {PIN_S0, PIN_S3, PIN_S2, PIN_S1};
const int outputPin = PIN_COM;

void setup() 
{
    Serial.begin( 115200 );
    pinMode( PIN_S0, OUTPUT );
    pinMode( PIN_S3, OUTPUT );
    pinMode( PIN_S2, OUTPUT );
    pinMode( PIN_S1, OUTPUT );
    pinMode( PIN_COM, OUTPUT );
}

void muxWrite( int whichChannel )
{
    for ( int inputPin = 0; inputPin < 4; inputPin++ )
    {
        int pinState = bitRead( whichChannel, inputPin );
        Serial.printf( "Status do bit %d do canal %d = %d\n", inputPin, whichChannel, pinState );
        
        digitalWrite( channel[inputPin], pinState );
        Serial.printf( "Escreveu no canal %d o valor %d\n", channel[inputPin], pinState );
    }
}

void loop() 
{
    int sequence[] = {0, 1, 8, 9, 4, 5, 12, 13 };
    //iterate through the first 8 channels of the multiplexer (change to 16 if you
    //have 16 LEDs or outputs connected)
    for ( int muxChannel = 0; muxChannel < 8; muxChannel++ )
    {
        //set the channel pins based on the channel you want
        //LED on - high - 100 milliseconds delay
        Serial.printf( "Selecionando o canal %d\n", sequence[muxChannel] );
        muxWrite( sequence[muxChannel] );
        Serial.printf( "Nivel ALTO\n", sequence[muxChannel] );
        digitalWrite( outputPin, HIGH );
        delay( 1000 );
        Serial.printf( "Nivel BAIXO\n", sequence[muxChannel] );
        digitalWrite( outputPin, LOW );
        delay( 1000 );
    } 
}
