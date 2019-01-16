#include <Arduino.h>
#include <WiFiClientSecure.h>

#define THINGER_SERVER "192.168.100.35"

#include <ThingerESP32.h>

#define USERNAME "vinicius"
#define DEVICE_ID "esp32_01"
#define DEVICE_CREDENTIAL "3FFh3n!z!Mpq"

#define SSID "qflux"
#define SSID_PASSWORD "33285901ctba"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
int _currentTemperature = 0, _currentHumidity = 0;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);

  thing.add_wifi(SSID, SSID_PASSWORD);

  // digital pin control example (i.e. turning on/off a light, a relay, configuring a parameter, etc)
  thing["led"] << digitalPin(LED_BUILTIN);

  // resource output example (i.e. reading a sensor value)
  thing["millis"] >> outputValue(millis());

  thing["weatherData"] >> [](pson &out)
  {
    out["temperature"] = _currentTemperature;
    out["humidity"] = _currentHumidity;
  };

  // more details at http://docs.thinger.io/arduino/
}

void loop() 
{
  thing.handle();
  _currentTemperature = ( temprature_sens_read() - 32 ) / 1.8;
  delay(100);
  _currentHumidity = random(100);
  delay(5000);
}
