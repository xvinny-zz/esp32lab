/*
 * Created: April 2016
 * By: Ab Kurk
 * Version:1
 * Description:
 * File Name:Interrupt_demo2.ino
 * This is part of a tutoral to show the difference between attaching an interrupt to a pin v.s. digitalRead(button1Pin). 
 * Attaching an interrupt to a hardware pin gives you the ability to handle inputs as they happen. This demo file has an interrupt
 * attacht to a hardware pin 2 instead of using the traditional digitalRead(button1Pin).
 * Link To Tutorial Page:http://www.thearduinomakerman.info/blog/2016/4/25/using-interrupts
 * 
 */


//Delcaring the button and LED Pins
#define button1Pin 35
#define led2Pin 33
int onoff = 0;//0 lights are off 1 light are on
volatile byte state = HIGH;

// the setup function runs once when you press reset or power the board
void setup() 
{
  pinMode( led2Pin, OUTPUT );
  pinMode( button1Pin, INPUT_PULLUP );
  Serial.begin( 115200 );
  state = LOW;
  digitalWrite( led2Pin, state );
  attachInterrupt( digitalPinToInterrupt( button1Pin ), blink1, CHANGE );
  
}
void blink1() 
{
  state = !state;
  digitalWrite( led2Pin, !state );
}

// the loop function runs over and over again forever
void loop() 
{
}
