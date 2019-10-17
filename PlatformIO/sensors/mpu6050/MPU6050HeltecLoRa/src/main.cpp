#include <Arduino.h>
#include <NtpClientLib.h>


void setup() 
{
    Serial.begin(115200);

    Serial.println("Iniciando o exemplo MPU6050...");
}

void loop() 
{
    Serial.printf("Crono: %lu\n", millis());
    delay(1000);
}