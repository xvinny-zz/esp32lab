#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/timer.h"
#include "gpioHandler.h"

#define GPIO_INPUT_PIN 		34
#define GPIO_OUTPUT_PIN		35
#define GPIO_PIN_LOW 	0
#define GPIO_PIN_HIGH 	1

#define TIMER_DIVIDER 			16
#define TIMER_SCALE				( TIMER_BASE_CLK / TIMER_DIVIDER )
#define TIMER_INTERVAL0_SEC		1.0

void io_task(void *pvParameter)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
	gpio_pad_select_gpio(GPIO_INPUT_PIN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GPIO_INPUT_PIN, GPIO_MODE_INPUT);

    gpio_pad_select_gpio(GPIO_OUTPUT_PIN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GPIO_OUTPUT_PIN, GPIO_MODE_OUTPUT);

    int level = GPIO_PIN_LOW;
    int currentLevel = GPIO_PIN_LOW;

    while(1)
    {
        level = gpio_get_level( GPIO_INPUT_PIN );

        if ( level != currentLevel )
        {
        	gpio_set_level( GPIO_OUTPUT_PIN, level );
        	currentLevel = level;
        }

        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
	ENGINE_STATE state;
	float externalBatt;
	float internalBatt;
	int externalBattRAW;
	int internalBattRAW;
	float engine;

	printf("Iniciando o aplicativo...");

    //xTaskCreate(&io_task, "io_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

	GPIOHandler_initialize();

	while(1)
	{
		state = GPIOHandler_getEngineState();
		//engine = GPIOHandler_readAnalogPin( PIN_ENGINE );
		externalBatt = GPIOHandler_readAnalogPin( PIN_EXTERNAL_BATTERY );
		internalBatt = GPIOHandler_readAnalogPin( PIN_INTERNAL_BATTERY );
		externalBattRAW = GPIOHandler_readAnalogPinRAW( PIN_EXTERNAL_BATTERY );
		internalBattRAW = GPIOHandler_readAnalogPinRAW( PIN_INTERNAL_BATTERY );

		ESP_LOGI( "[IOTEST]", "Motor: %s | Bat. Interna: (%.2f - Cru: %d) | Bat. Externa: (%.2f - Cru: %d)", state == ENGINE_STATE_ON ? "Ligado" : "Desligado", internalBatt, internalBattRAW, externalBatt, externalBattRAW );
		//ESP_LOGI( "[IOTEST]", "Motor: %.2f | Bat. Interna: (%.2f - Cru: %d) | Bat. Externa: (%.2f - Cru: %d)", engine, internalBatt, internalBattRAW, externalBatt, externalBattRAW );

		vTaskDelay(250 / portTICK_PERIOD_MS);
	}
}
