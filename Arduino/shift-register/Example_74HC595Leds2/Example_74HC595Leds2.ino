//Pin connected to ST_CP of 74HC595
int latchPin = 21;
//Pin connected to SH_CP of 74HC595
int clockPin = 4;
////Pin connected to DS of 74HC595
int dataPin = 15;
int oePin = 27;

//holders for infromation you're going to pass to shifting function
byte data;
byte dataArray[10];
int sum = 0;
byte leds[8] = { 0x01, 0x02, 0x08, 0x04, 0x10, 0x20, 0x80, 0x40 };

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  Serial.begin(115200);

  pinMode(oePin,OUTPUT);
  digitalWrite(oePin, LOW); 

  //Binary notation as comment
//  dataArray[0] = 0xFF; //0b11111111
//  dataArray[1] = 0xFE; //0b11111110
//  dataArray[2] = 0xFC; //0b11111100
//  dataArray[3] = 0xF4; //0b11110100
//  dataArray[4] = 0xF0; //0b11110000
//  dataArray[5] = 0xE0; //0b11100000
//  dataArray[6] = 0xC0; //0b11000000
//  dataArray[7] = 0x40; //0b01000000
//  dataArray[8] = 0x00; //0b00000000
//  dataArray[9] = 0xE0; //0b11100000

  dataArray[0] = 0x01; //0b00000001
  dataArray[1] = 0x02; //0b00000010
  dataArray[2] = 0x08; //0b00001000
  dataArray[3] = 0x04; //0b00000100
  dataArray[4] = 0x10; //0b00010000
  dataArray[5] = 0x20; //0b00100000
  dataArray[6] = 0x80; //0b10000000
  dataArray[7] = 0x40; //0b01000000
  dataArray[8] = 0x00; //0b00000000
  dataArray[9] = 0xFF; //0b11111111

  //function that blinks all the LEDs
  //gets passed the number of blinks and the pause time
  blinkAll_2Bytes(2,1000); 
}

void loop() {

  /*for (int j = 0; j < 10; j++) {
    //load the light sequence you want from array
    data = dataArray[j];
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, 0);
    //move 'em out
    shiftOut(dataPin, clockPin, data);
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
    delay(2000);
  }*/

//    for ( int i = 1 ; i <= 8 ; i++ )
//    {
//        setLed ( i , true );
//        delay( 2000 );
//        setLed ( i , false );
//        delay( 2000 );
//    }

    setLed ( 1 , true );
    setLed ( 8 , true ); delay( 2000 );
    
    setLed ( 2 , true );
    setLed ( 7 , true ); delay( 2000 );
    
    setLed ( 3 , true );
    setLed ( 6 , true ); delay( 2000 );
    
    setLed ( 4 , true );
    setLed ( 5 , true ); delay( 2000 );
    
    setLed ( 4 , false );
    setLed ( 5 , false ); delay( 2000 );
    
    setLed ( 3 , false );
    setLed ( 6 , false ); delay( 2000 );
    
    setLed ( 2 , false );
    setLed ( 7 , false ); delay( 2000 );
    
    setLed ( 1 , false );
    setLed ( 8 , false ); delay( 2000 );
    
}

bool setLed( int ledNumber , bool state )
{
    int ledIndex = ledNumber - 1;
    int currentSum = sum;
    Serial.printf( "Led Number: %d / State: %d\n", ledNumber , state );
    
    if ( ( ledNumber < 1 ) || ( ledNumber > 8 ) )
        return false;

    if ( state == false ) // desligar o led
    {
        if ( ( currentSum & leds[ledIndex] ) == leds[ledIndex] )
        {
            currentSum -= leds[ledIndex];
        }
    }
    else
    {
        if ( ( currentSum & leds[ledIndex] ) != leds[ledIndex] )
        {
            currentSum += leds[ledIndex];
        }
    }

    if ( currentSum != sum )
    {
        Serial.printf( "Sum: %d\n", currentSum );
        digitalWrite( latchPin, 0 );
        shiftOut( dataPin, clockPin, currentSum );
        digitalWrite( latchPin, 1 );
        sum = currentSum;
    }

    return true;
}


// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) 
{
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}


//blinks the whole register based on the number of times you want to 
//blink "n" and the pause between them "d"
//starts with a moment of darkness to make sure the first blink
//has its full visual effect.
void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}
