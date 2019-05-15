// lora_sleep.pde

// Caution:lora module can only work under 3.3V
// please make sure the supply of you board is under 3.3V
// 5v supply will destroy lora module!!

// This code runs in sleep mode

#include <Arduino.h>
#include <LORA.h>

#define PIN_NSS GPIO_NUM_10
#define PIN_NRESET GPIO_NUM_9
#define PIN_txEn GPIO_NUM_8
#define PIN_rxEn GPIO_NUM_7

LORA *lora;

void setup()
{
    Serial.begin(115200);

    LORA instance = LORA(PIN_NSS, PIN_NRESET, PIN_txEn, PIN_rxEn);
    *lora = instance;

    if (!lora->init())
    {
        Serial.println("Init fail!");
    }

    // enter sleep mode
    lora->enterSleepMode();
}

void loop()
{
}
