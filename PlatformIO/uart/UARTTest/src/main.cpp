#include <Arduino.h>

String inputBuffer;

void setup() 
{
    Serial.begin(115200);
    Serial2.begin(115200);

    inputBuffer = "";
    ESP_LOGI("MAIN", "Sistema iniciado. Aguardando comandos...");
  // put your setup code here, to run once:
}

void loop() 
{
    if (Serial2.available())
    {
        inputBuffer = Serial2.readStringUntil('\n');

        ESP_LOGI("MAIN", "Recebido da Serial2: [%s]", inputBuffer.c_str());
    }

    if (Serial.available())
    {
        inputBuffer = Serial.readStringUntil('\n');

        Serial2.printf("Recebido da Serial: [%s]", inputBuffer.c_str());
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  // put your main code here, to run repeatedly:
}