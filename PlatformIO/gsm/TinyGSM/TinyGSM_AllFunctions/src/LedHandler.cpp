/*
 * ledHandler.c
 *
 *  Created on: 7 de jun de 2018
 *      Author: Vinicius
 */

#include "LedHandler.h"

/**
 * Function: LedControllerClass
 * Description: this is the class constructor
 * Parameters: none
 * Returns: LedControllerClass
 */
LedControllerClass::LedControllerClass()
{
    m_isInitialized = false;
}
	
/**
 * Function: begin
 * Description: this function will initialize the LedControllerClass.
 * Parameters:
 *      -> gpio_num_t pinClock: GPIO which is connected to the MUX clock pin.
 *      -> gpio_num_t pinData: GPIO which is connected to the MUX data pin.
 *      -> gpio_num_t pinLoad: GPIO which is connected to the MUX load pin.
 *      -> gpio_num_t pinOE: GPIO which is connected to the MUX OE pin.
 * Returns: esp_err_t with the error code.
 */
esp_err_t LedControllerClass::begin(gpio_num_t pinClock, gpio_num_t pinData, gpio_num_t pinLoad, gpio_num_t pinOE)
{
    // Inicializando o Gerenciador de Leds.\n
    ESP_LOGI( TAG_LEDHANDLER , LOGI_0026 );
    
    m_pinClock = pinClock;
    m_pinData = pinData;
    m_pinLoad = pinLoad;
    m_pinOE = pinOE;

    pinMode( m_pinLoad, OUTPUT );
    pinMode( m_pinLoad, OUTPUT );
    digitalWrite( m_pinOE, LOW ); 

    m_dataSum = 0;

    m_isInitialized = true;
    
    return autoTest();
}

/**
 * Function: setLed
 * Description: turns a LED ON or OFF.
 * Parameters:
 *      -> LED led: the LED which will have the state changed.
 *      -> bool state: if true, turns the LED on, otherwise turns off.
 * Returns: esp_err_t with the error code.
 * Remarks: if the led is already on, this function does nothing and returns ESP_OK.
 */
esp_err_t LedControllerClass::setLed(LED led, bool state)
{
    if ( m_isInitialized == false )
  		return ESP_ERR_INVALID_STATE;
  
    int ledNumber = (int) led;
    int ledIndex = ledNumber - 1;
    int currentSum = m_dataSum;
  
    if ( ( ledNumber < 1 ) || ( ledNumber > 8 ) )
        return ESP_FAIL;
      
  	if ( state == false ) // desligar o led
    {
        if ( ( currentSum & LEDS[ledIndex] ) == LEDS[ledIndex] )
            currentSum -= LEDS[ledIndex];
    }
    else
    {
        if ( ( currentSum & LEDS[ledIndex] ) != LEDS[ledIndex] )
            currentSum += LEDS[ledIndex];
    }

    if ( currentSum != m_dataSum )
    {
        digitalWrite( m_pinLoad, LOW );
        shiftOut( m_pinData, m_pinClock, currentSum );
        digitalWrite( m_pinLoad, HIGH );
        m_dataSum = currentSum;
    }

  	return ESP_OK;
}

/**
 * Function: autoTest
 * Description: executes an auto test which allow you to check if the leds are working.
 * Parameters: none
 * Returns: esp_err_t with the error code.
 */
esp_err_t LedControllerClass::autoTest()
{
    esp_err_t result = ESP_OK;

    // Executando auto-teste dos leds.
    ESP_LOGI( TAG_LEDHANDLER , LOGI_0029 );
    
    LED currentLeds[] = { LED_ALARME_ESQ_2 , LED_ALARME_ESQ_1 , LED_ALERTA_ESQ , LED_SYSTEM_ON , LED_GPS , LED_ALERTA_DIR , LED_ALARME_DIR_1 , LED_ALARME_DIR_2 };

    // Ligando todos os leds.
    ESP_LOGI( TAG_LEDHANDLER , LOGI_0031 );
    for ( int i = 0 ; i < 8 ; i++ )
    {
        result = setLed( currentLeds[i], true );
        if (result != ESP_OK)
            return result;

        delay( 250 );
    }

    // Desligando todos os leds.
    ESP_LOGI( TAG_LEDHANDLER , LOGI_0030 );
    for ( int i = 0 ; i < 8 ; i++ )
    {
        result = setLed( currentLeds[i], false );
        if (result != ESP_OK)
            return result;
        delay( 250 );
    }

    return result;
}

/**
 * Function: shiftOut
 * Description: function This shifts 8 bits out MSB first, on the rising edge of the clock, clock idles low.
 * Parameters:
 *      -> gpio_num_t myDataPin: mux data pin.
 *      -> gpio_num_t myClockPin: mux clock pin.
 *      -> byte myDataOut: data to be written.
 * Returns: esp_err_t with the error code.
 */
esp_err_t LedControllerClass::shiftOut(gpio_num_t myDataPin, gpio_num_t myClockPin, byte myDataOut)
{
    //internal function setup
    int i = 0;
    int pinState;
    pinMode( myClockPin, OUTPUT );
    pinMode( myDataPin, OUTPUT );
    
    //clear everything out just in case to prepare shift register for bit shifting
    digitalWrite( myDataPin, LOW );
    digitalWrite( myClockPin, LOW );
    
    //for each bit in the byte myDataOut
    //NOTICE THAT WE ARE COUNTING DOWN in our for loop
    //This means that %00000001 or "1" will go through such
    //that it will be pin Q0 that lights. 
    for (i = 7; i >= 0; i-- )  
    {
        digitalWrite(myClockPin, LOW);
    
        //if the value passed to myDataOut and a bitmask result 
        // true then... so if we are at i=6 and our value is
        // %11010100 it would the code compares it to %01000000 
        // and proceeds to set pinState to 1.
        if ( myDataOut & (1<<i) ) 
            pinState= 1;
        else 
            pinState= 0;
    
        //Sets the pin to HIGH or LOW depending on pinState
        digitalWrite( myDataPin, pinState );
        //register shifts bits on upstroke of clock pin  
        digitalWrite( myClockPin, HIGH );
        //zero the data pin after shift to prevent bleed through
        digitalWrite( myDataPin, LOW );
    }
    
    //stop shifting
    digitalWrite( myClockPin, LOW );

    return ESP_OK;
}

LedControllerClass LedController;