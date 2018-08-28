#include <Arduino.h>
#include "ConnectionManager.h"

void setup() 
{
    ConnectionManager.begin();
}

void loop() 
{
    ESP_LOGI("MAIN", "Fazendo outra coisa...");
    delay(5000);
}