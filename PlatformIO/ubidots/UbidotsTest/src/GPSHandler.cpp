/************************************************************************
* Arquivo: GPSHandler.cpp
* Autor: Vinicius de Sá
* Data: 04/12/2017
* Objetivo: Contém a classe GPS que provê funções para uso do dispositivo GPS.
* Exemplo de código:
  
  GPS _gps( PIN_GPS_TX , PIN_GPS_RX );
  
  void setup()
  {
    ...
    _gps.begin();
    ...
  }

  void loop()
  {
      Serial.println( "[GPS]" + _gps.getString() );
  }
************************************************************************/
#include "GPSHandler.h"

#define SerialGPS Serial2

/* 
 *  Objetivo: Construtor da classe GPS. 
 *  Parâmetros:
 *    - byte gpsTX: pino de TX do dispositivo GPS.
 *    - byte gpsRX: pino de RX do dispositivo GPS.
*/
GPSClass::GPSClass()
{
}

/* 
 *  Método: begin
 *  Objetivo: Iniciar a comunicação com o GPS. 
 *  Parâmetros: nenhum
 *  Retorno: vazio
*/
void GPSClass::begin(gpio_num_t gpsTX, gpio_num_t gpsRX, unsigned long baudRate)
{
    // [GPS] Iniciando o GPS."
    ESP_LOGI("[GPS]", "Iniciando o GPS.");

    SerialGPS.begin(baudRate, SERIAL_8N1, (int8_t)gpsTX, (int8_t)gpsRX);
    _isInitialized = true;
    _hasFix = false;
    _messagesReadToFix = 0;
    _messagesReadToFixLost = 0;
    
    setSampleRate();

    ESP_LOGI("[GPS]", "GPS Iniciado.");
}

/* 
 *  Método: getData
 *  Objetivo: Obter os dados a partir do GPS.
 *  Parâmetros: nenhum
 *  Retorno: Objeto GPSContext contendo os dados do GPS.
*/
GPSContext GPSClass::getData()
{
    GPSContext context;

    context.Latitude = 0.0;
    context.Longitude = 0.0;
    context.Bearing = 0.0;
    context.Speed = 0.0;
    context.Altitude = 0.0;
    context.Time_Hours = 0;
    context.Time_Minutes = 0;
    context.Time_Seconds = 0;
    context.Time_Centiseconds = 0;
    context.Date_Year = 0;
    context.Date_Month = 0;
    context.Date_Day = 0;
    context.IsPositionUpdated = false;

    if (executeSmartDelay(1100) == false)
    {
        // GPS nao inicializado. Reiniciando o sistema em 5 segundos...
        Serial.println( "GPS nao inicializado. Reiniciando o sistema em 5 segundos..." );
        delay(5000);
        esp_restart();
    }

    if (millis() > 5000 && _tinyGPS.charsProcessed() < 10)
    {
        Serial.printf( "Nenhum dado recebido. Verifique o cabeamento." );
        context.IsValid = false;
        return context;
    }
    else
    {
        context.IsValid = true;
    }

    if (context.IsValid == true)
    {
        context.Satellites = _tinyGPS.satellites.value();
        context.HDOP = _tinyGPS.hdop.hdop();

        // Verificar se a coordenada eh valida.
        // Se tem 4 ou mais satelites e se o hdop esta abaixo de 5.
        if ( ( context.Satellites >= 4 ) && ( context.HDOP < 5 ) )
        {
             _messagesReadToFixLost = 0;

            if (_hasFix == false)
            {
                ++_messagesReadToFix;

                if (_messagesReadToFix >= 12)
                {
                    _hasFix = true;
                    _messagesReadToFix = 0;
                }
            }

            context.Latitude = _tinyGPS.location.lat();
            context.Longitude = _tinyGPS.location.lng();
            context.Bearing = _tinyGPS.course.deg();
            context.Speed = _tinyGPS.speed.kmph();
            context.Altitude = _tinyGPS.altitude.meters();
            context.Time_Hours = _tinyGPS.time.hour();
            context.Time_Minutes = _tinyGPS.time.minute();
            context.Time_Seconds = _tinyGPS.time.second();
            context.Time_Centiseconds = _tinyGPS.time.centisecond();
            context.Date_Year = _tinyGPS.date.year();
            context.Date_Month = _tinyGPS.date.month();
            context.Date_Day = _tinyGPS.date.day();
            context.IsPositionUpdated = !(( context.Latitude == 0.0 ) && ( context.Longitude == 0.0 ) && ( context.Speed == 0.0 ) && ( context.Bearing == 0.0 ));
        }
        else // context.Satellites >= 4
        {  
            _messagesReadToFix = 0;

            if (_hasFix == true)
            {
                ++_messagesReadToFixLost;
                if (_messagesReadToFixLost >= 12)
                {
                    _hasFix = false;
                    _messagesReadToFixLost = 0;
                }
            }
        }
    }

    return context;
}

/* 
 *  Método: getString
 *  Objetivo: Obter uma string contendo os dados do GPS.
 *  Parâmetros: nenhum
 *  Retorno: Objeto String contendo os dados do GPS.
*/
String GPSClass::getString()
{
    if (_isInitialized == false)
        return "GPS nao inicializado";

    GPSContext context = getData();

    if (context.IsValid == false)
        return "Dados de GPS recebidos mas invalidos.";

    String result = String(context.Latitude, 5) + ",";
    result = result + String(context.Longitude, 4) + ",";
    result = result + String(context.Bearing, 2) + ",";
    result = result + String(context.Speed, 2) + ",";
    result = result + String(context.Satellites) + ",";
    result = result + String(context.Altitude) + ",";
    if (context.Date_Day < 10)
        result = result + "0";
    result = result + String(context.Date_Day);
    if (context.Date_Month < 10)
        result = result + "0";
    result = result + String(context.Date_Month);
    if (context.Date_Year < 10)
        result = result + "0";
    result = result + String(context.Date_Year) + ",";
    if (context.Time_Hours < 10)
        result = result + "0";
    result = result + String(context.Time_Hours);
    if (context.Time_Minutes < 10)
        result = result + "0";
    result = result + String(context.Time_Minutes);
    if (context.Time_Seconds < 10)
        result = result + "0";
    result = result + String(context.Time_Seconds);
    if (context.Time_Centiseconds < 100)
        result = result + "0";
    if (context.Time_Centiseconds < 10)
        result = result + "0";
    result = result + String(context.Time_Centiseconds);

    return result;
}

/* 
 *  Método: executeSmartDelay
 *  Objetivo: Executar uma instrução de espera até o GPS retornar os próximos dados.
 *  Parâmetros: nenhum
 *  Retorno: Booleano informando se a operação foi válida.
*/
bool GPSClass::executeSmartDelay(unsigned long timeout)
{
    int incomingByte = 0;

    if (_isInitialized == false)
        return false;

    unsigned long start = millis();

    do
    {
        while (SerialGPS.available())
        {
            incomingByte = SerialGPS.read();
            _tinyGPS.encode(incomingByte);
            Serial.print((char)incomingByte);
        }

        if (_tinyGPS.time.isUpdated() == true)
            break;
    } while ((millis() - start) < timeout);

    if ((millis() - start) >= timeout)
    {
        if (_tinyGPS.time.isUpdated() == true)
        {
            // #define MSG_ERROR_GPS_02 "#999,01,02\n" // GPS timeout
            Serial.printf( "#999,01,02\n" );
        }
        else
        {
            // #define MSG_ERROR_GPS_01 "#999,01,01\n" // GPS sem sinal
            Serial.printf( "#999,01,01\n" );
        }
    }

    return true;
}

/****************************************************
 * Function: setSampleRate
 * Description: Set the GPS sample rate.
 * Parameters:
 *     -> uart_port_t uart: UART number.
 *    -> const char * command: command to be sent.
 *    -> size_t size: command size in bytes.
 * Returns:
 *    -> int: bytes written to TX FIFO .
 ****************************************************/
void GPSClass::setSampleRate()
{
    char command[35] = {
        // changes the sampling to 250ms
        0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xFA, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x96,
        // save settings
        0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1D, 0xAB};

    int result = SerialGPS.write((uint8_t *)command, sizeof(command));
    ESP_LOGI( TAG_GPS , LOGI_0072 , result );
}

bool GPSClass::hasFix()
{
    return _hasFix;
}

GPSClass GPS;