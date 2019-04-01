/**
 * @file main.cpp
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief 
 * @version 0.1
 * @date 01/04/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#include <Arduino.h>

#define DEVICE_1
#define MESSAGE "Ola, eu sou o device %d\n"

#ifdef DEVICE_1
const int DEVICE_ID = 1;
#define PIN_SERIAL_TX GPIO_NUM_17
#define PIN_SERIAL_RX GPIO_NUM_16
#else
const int DEVICE_ID = 2;
#define PIN_SERIAL_TX GPIO_NUM_14
#define PIN_SERIAL_RX GPIO_NUM_13
#endif
const int LOOP_INTERVAL = 1000;

HardwareSerial SerialComm(2);
unsigned long g_currentTime;
String g_incoming;

void setup() 
{
    Serial.begin(115200);
    delay(2000);

    g_currentTime = millis();
    ESP_LOGD("MAIN", "Inicializando o device %d a %lu ms", DEVICE_ID, g_currentTime);

    SerialComm.begin(115200, SERIAL_8N1, PIN_SERIAL_RX, PIN_SERIAL_TX);        

    ESP_LOGD("MAIN", "Device inicializado! Lendo/escrevendo mensagens...");
}

void loop() 
{
    g_incoming = "";

    if (SerialComm.available())
    {
        while (SerialComm.available())
        {
            g_incoming.concat((char)SerialComm.read());
        }

        g_incoming.trim();

        if (g_incoming.length() >= strlen(MESSAGE))
        {
            ESP_LOGD("MAIN", "[Recebido] %s", g_incoming.c_str());
        }
    }

    SerialComm.printf(MESSAGE, DEVICE_ID);
    ESP_LOGD("MAIN", "[Enviado] Ola, eu sou o device %d", DEVICE_ID );

    delay(LOOP_INTERVAL);
}