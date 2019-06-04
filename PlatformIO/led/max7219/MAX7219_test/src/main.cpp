//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
//LedControl lc = LedControl(23, 18, 5, 1);

#define DISPLAY_1_DIN 25
#define DISPLAY_1_CLK 22
#define DISPLAY_1_CS 5

LedControl lc1 = LedControl(DISPLAY_1_DIN, DISPLAY_1_CLK, DISPLAY_1_CS, 1);

#define DISPLAY_2_DIN 18
#define DISPLAY_2_CLK 26
#define DISPLAY_2_CS 23

LedControl lc2 = LedControl(DISPLAY_2_DIN, DISPLAY_2_CLK, DISPLAY_2_CS, 1);

/* we always wait a bit between updates of the display */
unsigned long delaytime = 250;

void write8sOn7Segment(LedControl *lc);
void writeArduinoOn7Segment( LedControl *lc );
void scrollDigits( LedControl *lc );

void setup()
{
    Serial.begin(115200);
    /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
    lc1.shutdown(0, false);
    /* Set the brightness to a medium values */
    lc1.setIntensity(0, 15);
    /* and clear the display */
    lc1.clearDisplay(0);
    lc2.shutdown(0, false);
    /* Set the brightness to a medium values */
    lc2.setIntensity(0, 15);
    /* and clear the display */
    lc2.clearDisplay(0);

    write8sOn7Segment(&lc1);
    write8sOn7Segment(&lc2);
}

void loop()
{
    // writeArduinoOn7Segment(&lc1);
    // scrollDigits(&lc1);
    // writeArduinoOn7Segment(&lc2);
    // scrollDigits(&lc2);
}

void write8sOn7Segment(LedControl *lc)
{
    lc->clearDisplay(0);
    lc->setDigit(0, 7, 8, false);
    lc->setDigit(0, 6, 8, false);
    lc->setDigit(0, 5, 8, false);
    lc->setDigit(0, 4, 8, false);
    lc->setDigit(0, 3, 8, false);
    lc->setDigit(0, 2, 8, false);
    lc->setDigit(0, 1, 8, false);
    lc->setDigit(0, 0, 8, false);
}



/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment( LedControl *lc )
{
    lc->setChar(0, 0, 'a', false);
    delay(delaytime);
    lc->setRow(0, 0, 0x05);
    delay(delaytime);
    lc->setChar(0, 0, 'd', false);
    delay(delaytime);
    lc->setRow(0, 0, 0x1c);
    delay(delaytime);
    lc->setRow(0, 0, B00010000);
    delay(delaytime);
    lc->setRow(0, 0, 0x15);
    delay(delaytime);
    lc->setRow(0, 0, 0x1D);
    delay(delaytime);
    lc->clearDisplay(0);
    delay(delaytime);
}

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits( LedControl *lc )
{
    for (int i = 0; i < 13; i++)
    {
        lc->setDigit(0, 3, i, false);
        lc->setDigit(0, 2, i + 1, false);
        lc->setDigit(0, 1, i + 2, false);
        lc->setDigit(0, 0, i + 3, false);
        lc->setDigit(0, 7, i, false);
        lc->setDigit(0, 6, i + 1, false);
        lc->setDigit(0, 5, i + 2, false);
        lc->setDigit(0, 4, i + 3, false);
        delay(delaytime);
    }
    lc->clearDisplay(0);
    delay(delaytime);
}