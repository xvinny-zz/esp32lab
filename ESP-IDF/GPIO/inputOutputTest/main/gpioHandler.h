/*
 * gpioHandler.h
 *
 *  Created on: 6 de mar de 2018
 *      Author: Vinicius
 */

#ifndef MAIN_GPIOHANDLER_H_
#define MAIN_GPIOHANDLER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "messageLibrary.h"
#include "esp_log.h"
#include "esp_err.h"

///////////////////////////////////////////////////
///		 		DEFINIÇÕES DOS PINOS			///
///////////////////////////////////////////////////
#define PIN_ENGINE							GPIO_NUM_34
#define ADC_CHANNEL_ENGINE					ADC1_CHANNEL_6

//#define PIN_EXTERNAL_BATTERY				GPIO_NUM_33
//#define ADC_CHANNEL_EXTERNAL_BATTERY		ADC1_CHANNEL_5

//#define PIN_INTERNAL_BATTERY 				GPIO_NUM_32
//#define ADC_CHANNEL_INTERNAL_BATTERY		ADC1_CHANNEL_4

#define PIN_INTERNAL_BATTERY				GPIO_NUM_35
#define ADC_CHANNEL_INTERNAL_BATTERY		ADC1_CHANNEL_7

#define PIN_EXTERNAL_BATTERY 		 		GPIO_NUM_36			// VP
#define ADC_CHANNEL_EXTERNAL_BATTERY		ADC1_CHANNEL_0




#define ESP_INTR_FLAG_DEFAULT 			0

///////////////////////////////////////////////////
///		 			TYPEDEFS					///
///////////////////////////////////////////////////
typedef enum
{
	ENGINE_STATE_OFF = 0, ENGINE_STATE_ON = 1
} ENGINE_STATE;

/*
 * Delegate: ButtonPressedEventHandler
 * Objetivo: define a assinatura da função para pressionamento do botão.
 * Parâmetros:
 * 		-> bool: nível do pino.
 * Retorno: vazio
 */
typedef void ( *LevelChangedEventHandler )();

void GPIOHandler_initialize();
ENGINE_STATE GPIOHandler_getEngineState();
float GPIOHandler_readAnalogPin( int number );
int GPIOHandler_readAnalogPinRAW( int number );


#endif /* MAIN_GPIOHANDLER_H_ */
