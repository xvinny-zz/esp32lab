#include <Arduino.h>
#include <NeoPixelBrightnessBus.h>
#include "LedControl.h"

#define LED_COUNT       100
#define LED_DATA_PIN    GPIO_NUM_33

#define Color_t RgbColor

#define DISPLAY_1_DIN 25
#define DISPLAY_1_CLK 22
#define DISPLAY_1_CS 5

#define DISPLAY_2_DIN 18
#define DISPLAY_2_CLK 26
#define DISPLAY_2_CS 23

#define colorSaturation 255 // saturation of color constants
const Color_t LED_COLOR_RED = Color_t(HtmlColor(0xFF0000));
const Color_t LED_COLOR_GREEN = Color_t (HtmlColor(0x008000));
const Color_t LED_COLOR_BLUE = Color_t (HtmlColor(0x0000FF));
const Color_t LED_COLOR_YELLOW = Color_t(HtmlColor(0xFFFF00));
const Color_t LED_COLOR_WHITE = Color_t(HtmlColor(0xFFFFFF));
const Color_t LED_OFF = Color_t(0,0,0);
const uint8_t c_MinBrightness = 8; 
const uint8_t c_MaxBrightness = 255;

LedControl lc1 = LedControl(DISPLAY_1_DIN, DISPLAY_1_CLK, DISPLAY_1_CS, 1);
LedControl lc2 = LedControl(DISPLAY_2_DIN, DISPLAY_2_CLK, DISPLAY_2_CS, 1);

NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod> strip(LED_COUNT, LED_DATA_PIN);
Color_t m_currentColors[LED_COUNT];
int8_t direction; // current direction of dimming

void updateMatrixAndShow()
{
    for (int i = 0 ; i < LED_COUNT ; i++)
    {
        strip.SetPixelColor(i, m_currentColors[i]);
    }

    strip.Show();
}

/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment( LedControl *lc )
{
    lc->clearDisplay(0);

    lc->setDigit(0, 0, 8, false);
    lc->setDigit(0, 1, 8, false);
    lc->setDigit(0, 2, 8, false);
    lc->setDigit(0, 3, 8, false);
    lc->setDigit(0, 4, 8, false);
    lc->setDigit(0, 5, 8, false);
    lc->setDigit(0, 6, 8, false);
    lc->setDigit(0, 7, 8, false);
}

void setup()
{
    // put your setup code here, to run once:
    strip.Begin();
    strip.Show();

    for (int i = 0 ; i < 20 ; i++)
        m_currentColors[i] = LED_COLOR_RED;
    
    for (int i = 20 ; i < 40 ; i++)
        m_currentColors[i] = LED_COLOR_GREEN;

    for (int i = 40 ; i < 60 ; i++)
        m_currentColors[i] = LED_COLOR_BLUE;

    for (int i = 60 ; i < 80 ; i++)
        m_currentColors[i] = LED_COLOR_YELLOW;

    for (int i = 80 ; i < LED_COUNT ; i++)
        m_currentColors[i] = LED_COLOR_WHITE;
    
    updateMatrixAndShow();

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

    writeArduinoOn7Segment(&lc1);
    writeArduinoOn7Segment(&lc2);

    direction = -1; // default to dim first
}

void loop()
{
    // uint8_t brightness = strip.GetBrightness();
    // Serial.println(brightness);
    
    // delay(100);

    // // swap diection of dim when limits are reached
    // //
    // if (direction < 0 && brightness <= c_MinBrightness)
    // {
    //   direction = 1;
    // }
    // else if (direction > 0 && brightness >= c_MaxBrightness)
    // {
    //   direction = -1;
    // }
    // // apply dimming
    // brightness += direction;
    // strip.SetBrightness(brightness);

    // // show the results
    // strip.Show();
}