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

/*
#define PIN_LATCH 	8 	// Pin connected to ST_CP of 74HC595
#define PIN_CLOCK 	12 	// Pin connected to SH_CP of 74HC595
#define PIN_DATA 	11  // Pin connected to DS of 74HC595
*/

//#define PIN_CLOCK   4     // Pin connected to SH_CP of 74HC595 SRCLK
//#define LED_PIN     15    // Pin OE
//#define PIN_DATA    27    // Pin connected to DS of 74HC595
//#define PIN_LOAD 	  21 	  // Pin connected to ST_CP of 74HC595

#define PIN_CLOCK   4     // Pin connected to SH_CP of 74HC595 SRCLK
#define LED_PIN     27    // Pin OE
#define PIN_DATA    15    // Pin connected to DS of 74HC595
#define PIN_LOAD     21    // Pin connected to ST_CP of 74HC595


#define LEDC_CHANNEL_0        0
#define LEDC_TIMER_RESOLUTION 8
#define LEDC_BASE_FREQ        10000


#define LSBFIRST 0
#define MSBFIRST 1

#define LOW 0
#define HIGH 1

typedef enum
{
	LED_OFF = 0,
	LED_ALARME_ESQ = 1,
	LED_ALERTA_ESQ = 2,
	LED_GPS = 4,
	LED_4 = 8,
	LED_5 = 16,
	LED_6 = 32,
	LED_ALERTA_DIR = 128,
	LED_ALARME_DIR = 64
}LED;

typedef enum
{
	LED_STATE_ON,
	LED_STATE_OFF,
	LED_STATE_BLINKING
}LED_STATE;

/**
 * Fun��o: LedHandler_Initialize
 * Objetivo: inicializar o manipulador de leds.
 * Par�metros: nenhum
 * Retorno: esp_err_t
 */
esp_err_t LedHandler_Initialize();

/**
 * Fun��o: LedHandler_SetLed
 * Objetivo: configurar um led para um determinado valor.
 * Par�metros:
 * 		-> LED led: led a ser configurado.
 * 		-> LED_STATE: estado do led a ser configurado.
 * Retorno: esp_err_t
 */
esp_err_t LedHandler_SetLed( LED led , LED_STATE state );

void LedHandler_AutoTest();

#endif /* MAIN_LEDHANDLER_H_ */
