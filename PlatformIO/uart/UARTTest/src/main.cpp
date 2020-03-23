#include <Arduino.h>

#define SerialAT Serial1

const gpio_num_t GSM_TX = GPIO_NUM_13;
const gpio_num_t GSM_RX = GPIO_NUM_14;

String inputBuffer;
String outputBuffer;


void setup() 
{
    Serial.begin(115200);
    SerialAT.begin(115200, SERIAL_8N1, GSM_TX, GSM_RX, false);
    delay(500);

    inputBuffer = "";
    outputBuffer = "";
    ESP_LOGI("MAIN", "Sistema iniciado. Aguardando comandos...");
  // put your setup code here, to run once:
}

void loop() 
{
    while(Serial.available())
        outputBuffer.concat((char)Serial.read());

    if (outputBuffer.length() > 0)
    {
        SerialAT.print(outputBuffer);
        outputBuffer = "";
    }

    while (SerialAT.available())
        inputBuffer.concat((char)SerialAT.read());

    if (inputBuffer.length() > 0)
    {
        Serial.print(inputBuffer);
        inputBuffer = "";
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
}