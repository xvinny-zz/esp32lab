#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ArduinoJson.h>
#include "OTAManager.h"
#include "ConnectionManager.h"

//#define GPIO_LED_BLUE   2
//#define GPIO_LED_RED    14

const char* host = "esp32";
const char* ssid = "qflux";
const char* password = "33285901ctba";


/*
 * setup function
 */
void setup(void) 
{
    Serial.begin(115200);

    ConnectionManager.begin();
    
    OTAManager.begin();
}

void loop(void) 
{
    ESP_LOGI("LOOP", "Loop process v16...");
    delay(2000);
}
