#include <Arduino.h>


//#define SerialAux Serial1
#define SerialAux Serial2

const gpio_num_t GSM_TX = GPIO_NUM_13;
const gpio_num_t GSM_RX = GPIO_NUM_14;
// const gpio_num_t GPS_TX = GPIO_NUM_16;
// const gpio_num_t GPS_RX = GPIO_NUM_17;

String bufferSerialAux;
String bufferSerial;


void setup() 
{
    Serial.begin(115200);
    //SerialAux.begin(115200, SERIAL_8N1, GSM_TX, GSM_RX, false);
    SerialAux.begin(115200, SERIAL_8N1, GSM_TX, GSM_RX, false);
    delay(500);

    bufferSerialAux = "";
    bufferSerial = "";
    ESP_LOGI("MAIN", "Sistema iniciado. Aguardando comandos...");
  // put your setup code here, to run once:
}

void loop() 
{
    while(Serial.available())
        bufferSerial.concat((char)Serial.read());

    if (bufferSerial.length() > 0)
    {
        SerialAux.print(bufferSerial.c_str());
        //Serial.printf("\nEnviado: %s", bufferSerial.c_str());
        bufferSerial = "";
    }

    while (SerialAux.available())
        bufferSerialAux.concat((char)SerialAux.read());

    if (bufferSerialAux.length() > 0)
    {
        Serial.print(bufferSerialAux.c_str());
        //SerialAux.printf("\nEnviado: %s", bufferSerialAux.c_str());
        bufferSerialAux = "";
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
}