#include <Arduino.h>
#include "Engine.h"

void setup() 
{
    Serial.begin(115200);
    Engine.begin();
}

void loop() 
{
    Engine.handle();
    delay(10);
}
