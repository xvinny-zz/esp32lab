/*
 * ledHandler.h
 *
 *  Created on: 7 de jun de 2018
 *      Author: Vinicius
 */

#ifndef MAIN_LEDHANDLER_H_
#define MAIN_LEDHANDLER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "sdkconfig.h"
#include "Arduino.h"
#include "MessageLibrary.h"

const byte LEDS[8] = { 0x01, 0x02, 0x08, 0x04, 0x10, 0x20, 0x80, 0x40 };

typedef enum
{
	LED_ALARME_ESQ_2 = 1,
	LED_ALARME_ESQ_1 = 2,
	LED_ALERTA_ESQ = 3,
	LED_SYSTEM_ON = 4,
	LED_GPS = 5,
	LED_ALERTA_DIR = 6,
	LED_ALARME_DIR_1 = 7,
	LED_ALARME_DIR_2 = 8
}LED;

class LedControllerClass
{
	public:
		/**
		 * Function: LedControllerClass
		 * Description: this is the class constructor
		 * Parameters: none
		 * Returns: LedControllerClass
		 */
		LedControllerClass();
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
		esp_err_t begin(gpio_num_t pinClock = GPIO_NUM_4, 
						gpio_num_t pinData = GPIO_NUM_15, 
						gpio_num_t pinLoad = GPIO_NUM_21,
						gpio_num_t pinOE = GPIO_NUM_27);
		/**
		 * Function: setLed
		 * Description: turns a LED ON or OFF.
		 * Parameters:
		 *      -> LED led: the LED which will have the state changed.
		 *      -> bool state: if true, turns the LED on, otherwise turns off.
		 * Returns: esp_err_t with the error code.
		 * Remarks: if the led is already on, this function does nothing and returns ESP_OK.
		 */
		esp_err_t setLed(LED led, bool state);
	private:
		/**
		 * Function: autoTest
		 * Description: executes an auto test which allow you to check if the leds are working.
		 * Parameters: none
		 * Returns: esp_err_t with the error code.
		 */
		esp_err_t autoTest();
		/**
		 * Function: shiftOut
		 * Description: function This shifts 8 bits out MSB first, on the rising edge of the clock, clock idles low.
		 * Parameters:
		 *      -> gpio_num_t myDataPin: mux data pin.
		 *      -> gpio_num_t myClockPin: mux clock pin.
		 *      -> byte myDataOut: data to be written.
		 * Returns: esp_err_t with the error code.
		 */
		esp_err_t shiftOut(gpio_num_t myDataPin, gpio_num_t myClockPin, byte myDataOut);
		// ---------------Member Variables-------------	
		gpio_num_t m_pinClock; 
		gpio_num_t m_pinData;
		gpio_num_t m_pinLoad;
		gpio_num_t m_pinOE;
		bool m_isInitialized;
		int m_dataSum;
};

extern LedControllerClass LedController;

#endif /* MAIN_LEDHANDLER_H_ */
