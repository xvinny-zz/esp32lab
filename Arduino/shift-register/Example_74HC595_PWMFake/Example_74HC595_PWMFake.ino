
/* ***********************************
* Pseudo PWM of 8 LEDs through a
* 74HC595 Bit Shift register
* By Slugsie - 19th Nov 2009
* ***********************************/

// Setup pin definitions for bit shift register
int dataPin = 22;
int clockPin = 4;
int latchPin = 21;

// Setup array to hold brightnesses for each LED
// Start each LED off with a different brightness
int ledBrightness[] = {0, 1, 2, 3, 4, 0, 1, 2};

// Delay time between each cycle - smaller the better to reduce flicker
int delayTime = 1;

void setup() {
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(latchPin, OUTPUT);
}

void loop() 
{
	// Cycle through the brightness pattern BIT by BIT 256 times
	for (int iLoop = 0; iLoop <= 256; iLoop++) 
	{
		// Stores the BIT pattern for the run
		byte bitPattern = 0;
		// Cycle through each LED
		for (int loopLED = 0; loopLED <=7; loopLED++) {
			bitPattern = bitPattern | getBit(loopLED);
		}
		digitalWrite(latchPin, LOW);
		shiftOut(dataPin, clockPin, MSBFIRST, bitPattern);
		digitalWrite(latchPin, HIGH);
		incrementPWMPosition();
		delay(delayTime);
	}

	// Cycle through brightnesslevels 0 thru 4 for each LED
	for (int loopLED2 = 0; loopLED2 <=7; loopLED2++) 
	{
		ledBrightness[loopLED2]++;
		if (ledBrightness[loopLED2]>4) {
			ledBrightness[loopLED2] = 0;
		}
	}
}

// Increment the position within the brightnessPattern[] from 0 thru 7
int pwmPosition = 0;

void incrementPWMPosition() 
{
	pwmPosition++;
	if (pwmPosition > 7) {
		pwmPosition = 0;
	}
}

// Pattern of BITs that we use to determine if an LED gets power this cycle or not
// The more 1 BITs in the pattern, the brighter the LED will be overall.
// The 1's are spread as much as possible in order to try and reduce flicker.
byte brightnessPattern[] = {B00000000, B00000001, B00010001, B01010101, B11111111};


// BIT mask used to look at an individual BIT in the brightnessPattern[],
// also to set the individual BIT for the LED
byte bitMask[] = {B00000001, B00000010, B00000100, B00001000,
	B00010000, B00100000, B01000000, B10000000
};

byte getBit(int led) 
{
	if (brightnessPattern[ledBrightness[led]] & bitMask[pwmPosition]) {
		return bitMask[led];
	}
	else {
		return 0;
	}
}
