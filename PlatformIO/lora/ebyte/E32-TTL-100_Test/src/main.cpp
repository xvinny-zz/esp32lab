#include <Arduino.h>
#include "E32TTL100.h"

#define SerialLoRa Serial2
#define PIN_LORA_AUX GPIO_NUM_19
#define PIN_LORA_M0 GPIO_NUM_27
#define PIN_LORA_M1 GPIO_NUM_25
#define PIN_LORA_RX GPIO_NUM_14 // O RX do modulo esta ligado no pino IO14 do ESP32
#define PIN_LORA_TX GPIO_NUM_13 // O TX do modulo esta ligado no pino IO13 do ESP32

void setup()
{
    Serial.begin(115200);
    
    RET_STATUS result;

    MODE_TYPE loRaInitialMode = MODE_0_NORMAL;
    E32_PIN_t pins = {
        .Aux = PIN_LORA_AUX,
        .M0 = PIN_LORA_M0,
        .M1 = PIN_LORA_M1};

    SerialLoRa.begin( SERIAL_BAUD_RATE , SERIAL_8N1, PIN_LORA_TX, PIN_LORA_RX, false );

    result = E32TTL100.begin(pins, &SerialLoRa, loRaInitialMode);
    if (result != RET_SUCCESS)
    {
        ESP_LOGE("MAIN", "Erro ao inicializar o E32TTL100. Mensagem: %s", E32TTL100.getErrorMessage(result).c_str());
        ESP.restart();
    }
}

void loop()
{
    uint8_t dataBuffer[100], dataLength;
    String receivedMessage = "";

    if(E32TTL100.receiveMessage(dataBuffer, &dataLength) == RET_SUCCESS)
    {
        if (dataLength > 0)
        {
            for (int i = 0 ; i < dataLength ; i++)
            {
                receivedMessage = receivedMessage + String(dataBuffer[i], HEX) + " ";
            }
        }
        ESP_LOGI( TAG_E32, "Mensagem recebida: %s", receivedMessage.c_str());
    }

    E32TTL100.sendMessage();

    delay(1000);
}