/*
 * gpioHandler.c
 *
 *  Created on: 6 de mar de 2018
 *      Author: Vinicius
 */
#include "gpioHandler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "messageLibrary.h"
#include "esp_log.h"
#include "esp_err.h"

SemaphoreHandle_t xSemaphore = NULL;
SemaphoreHandle_t xSemaphoreTrip = NULL;
static int input1Level = 0;

void input_task(void* arg)
{
	int level;

    for(;;)
    {
    	level = gpio_get_level( PIN_ENGINE );
//    	if (level == 1)
//    		level = 0;
//    	else if (level == 0)
//    		level = 1;

        if( level != input1Level )
        {
        	xSemaphoreTake( xSemaphoreTrip, portMAX_DELAY );
        	input1Level = level;
			xSemaphoreGive( xSemaphoreTrip );

			printf("\n[GPIOHANDLER] Engine Started = %d\n", level );
        }

        vTaskDelay( 10 / portTICK_PERIOD_MS );
    }
}

/**
 * Função: GPIOHandler_initialize
 * Objetivo: inicializar o GPIO Handler.
 */
void GPIOHandler_initialize()
{
	xSemaphoreTrip = xSemaphoreCreateMutex();

	// "Inicializando o controlador da GPIO."
	ESP_LOGI( TAG_GPIO, LOGI_0047 );

	adc1_config_width( ADC_WIDTH_9Bit );
	adc1_config_channel_atten( ADC_CHANNEL_EXTERNAL_BATTERY, ADC_ATTEN_DB_11 );
	adc1_config_channel_atten( ADC_CHANNEL_INTERNAL_BATTERY, ADC_ATTEN_DB_11 );
	adc1_config_channel_atten( ADC_CHANNEL_ENGINE, ADC_ATTEN_DB_11 );

//	gpio_pad_select_gpio( PIN_ENGINE );
//	gpio_set_direction( PIN_ENGINE, GPIO_MODE_INPUT );
//	xTaskCreate( input_task, "input_task", 2048, NULL, 10, NULL );

	// "GPIO inicializado."
	ESP_LOGI( TAG_GPIO, LOGI_0048 );
}

/**
 * FunçãoÇ GPIOHandler_getInput1Level
 * Objetivo: obter nível da entrada 1 (pino 34) que se refere ao motor do veículo estar ligado ou desligado.
 * Retorno: inteiro (0 - desligado; 1 - ligado)
 */
ENGINE_STATE GPIOHandler_getEngineState()
{
	/*int level = 0;

	xSemaphoreTake( xSemaphoreTrip, portMAX_DELAY );
	level = input1Level;
	xSemaphoreGive( xSemaphoreTrip );

	return (ENGINE_STATE) level;*/

	float level = GPIOHandler_readAnalogPin( PIN_ENGINE );
	return (level > 100) ? ENGINE_STATE_ON : ENGINE_STATE_OFF;
}

float GPIOHandler_readAnalogPin( int number )
{
	float level = 0;

	switch(number)
	{
	case PIN_EXTERNAL_BATTERY:
		level = adc1_get_raw( ADC_CHANNEL_EXTERNAL_BATTERY );
		if (level < 55.44 )
			level = 0;
		else
			level = ( ( ( level / 27.72 ) + 1 ) * 1.01 );
		break;
	case PIN_INTERNAL_BATTERY:
		level = adc1_get_raw( ADC_CHANNEL_INTERNAL_BATTERY );
		if (level < 55.44 )
			level = 0;
		else
			level = ( ( ( level / 27.72 ) + 1 ) * 1.01 );
		break;
	case PIN_ENGINE:
		level = adc1_get_raw( ADC_CHANNEL_ENGINE );
		break;
	default:
		level = -1;
		break;
	}

	return level;
}

int GPIOHandler_readAnalogPinRAW( int number )
{
	int level = 0;

	switch(number)
	{
	case PIN_EXTERNAL_BATTERY:
		level = adc1_get_raw( ADC_CHANNEL_EXTERNAL_BATTERY );
		break;
	case PIN_INTERNAL_BATTERY:
		level = adc1_get_raw( ADC_CHANNEL_INTERNAL_BATTERY );
		break;
	default:
		level = -1;
		break;
	}

	return level;

}
