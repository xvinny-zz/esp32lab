/*
 * ledHandler.c
 *
 *  Created on: 7 de jun de 2018
 *      Author: Vinicius
 */

#include "ledHandler.h"

char dataArray[10];
int ledSum = 0;
bool isLedHandlerInitialized = false;

void LedHandler_AutoTest()
{ 
    // Executando auto-teste dos leds.
    Serial.printf( "%s: %s", "LEDHANDLER" , "Executando auto-teste dos leds." );
    
    LED leds[] = { LED_ALARME_ESQ , LED_ALERTA_ESQ , LED_GPS , LED_4 , LED_5 , LED_6 , LED_ALERTA_DIR , LED_ALARME_DIR , LED_OFF };
  
    for ( int i = 0 ; i < 8 ; i++ )
    {
        LedHandler_SetLed( leds[i], LED_STATE_ON );
        delay( 100 );
    }
  
    for ( int i = 0 ; i < 8 ; i++ )
    {
        LedHandler_SetLed( leds[i], LED_STATE_OFF );
        delay( 100 );
    }
}

/**
 * Funcao: LedHandler_Initialize
 * Objetivo: inicializar o manipulador de leds.
 * Parametros: nenhum
 * Retorno: esp_err_t
 */
esp_err_t LedHandler_Initialize()
{
  // Executando auto-teste dos leds.
  Serial.printf( "%s: %s", "LEDHANDLER" , "Inicializando o Gerenciador de Leds." );
  
	pinMode( PIN_DATA  , OUTPUT );
  pinMode( PIN_LOAD  , OUTPUT );
  pinMode( PIN_CLOCK , OUTPUT );

  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_RESOLUTION);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
  
	isLedHandlerInitialized = true;

  LedHandler_AutoTest();
 
	return ESP_OK;
}

/**
 * Funcao: LedHandler_SetLed
 * Objetivo: configurar um led para um determinado valor.
 * Parametros:
 * 		-> LED led: led a ser configurado.
 * 		-> LED_STATE: estado do led a ser configurado.
 * Retorno: esp_err_t
 */
esp_err_t LedHandler_SetLed( LED led , LED_STATE state )
{
	if ( isLedHandlerInitialized == false )
		return ESP_ERR_INVALID_STATE;

	switch ( state )
	{
		case LED_STATE_ON:
			if ( ( ledSum & (uint8_t) led ) == (uint8_t) led )
			{
          // O LED %d ja foi ligado.
          Serial.printf( "%s: %s %d %s", "LEDHANDLER" , "O LED" , (uint8_t) led, "ja foi ligado." );
			}
			else
			{
				  ledSum += (uint8_t) led;
			}
			break;
		case LED_STATE_OFF:
			if ( ( ledSum & (uint8_t) led ) != (uint8_t) led )
			{
          // O LED %d ja foi ligado.
          Serial.printf( "%s: %s %d %s", "LEDHANDLER" , "O LED" , (uint8_t) led, "ja foi desligado." );
			}
			else
			{
				  ledSum -= (uint8_t) led;
			}
			break;
		default:
			break;
	}

  digitalWrite( PIN_LOAD , LOW );
  shiftOut( PIN_DATA, PIN_CLOCK , MSBFIRST , ledSum );
  digitalWrite( PIN_LOAD , HIGH );
  ledcWrite( LEDC_CHANNEL_0, 200 );

	return ESP_OK;
}
