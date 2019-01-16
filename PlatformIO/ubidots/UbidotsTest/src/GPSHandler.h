/************************************************************************
* Arquivo: GPSHandler.h
* Autor: Vinicius de Sá
* Data: 04/12/2017
* Objetivo: Contém a classe GPS que provê funções para uso do dispositivo GPS.
************************************************************************/
#ifndef __GPS_HANDLER_H__

#define __GPS_HANDLER_H__

#include <TinyGPS++.h>
#include "esp_log.h"

const unsigned long GPS_BAUD_RATE = 9600;  // Baudrate = 9600 bps

struct GPSContext
{
    double Latitude;
    double Longitude;
    double Bearing;
    double Altitude;
    double Speed;
    uint8_t Time_Hours;
    uint8_t Time_Minutes;
    uint8_t Time_Seconds;
    uint8_t Time_Centiseconds;
    uint16_t Date_Year;
    uint8_t Date_Month;
    uint8_t Date_Day;
    uint32_t Satellites;
    double HDOP;
    bool IsValid;
    bool IsPositionUpdated;
};

class GPSClass
{
    public:
        GPSClass();
        void begin( gpio_num_t gpsTX, gpio_num_t gpsRX, unsigned long baudRate = 9600);
        bool isLocationValid();
        bool isLocationUpdated();
        double getLatitude();
        double getLongitude();
        uint16_t getYear();
        String getString();
        GPSContext getData();
        bool hasFix();
        void setSampleRate();
    private:
        TinyGPSPlus _tinyGPS;
        bool _isInitialized;  
        bool _hasFix;  
        int _messagesReadToFix;    
        int _messagesReadToFixLost;    
        bool executeSmartDelay( unsigned long timeout );
};

extern GPSClass GPS;
#endif //__GPS_HANDLER_H__
