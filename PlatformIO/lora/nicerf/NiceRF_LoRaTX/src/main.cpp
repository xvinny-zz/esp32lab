// lora_tx.pde

// Caution:lora module can only work under 3.3V
// please make sure the supply of you board is under 3.3V
// 5v supply will destroy lora module!!

// This code runs in tx mode and  works with lora_rx.pde
// Flow:receive "T" from serial->send a packet
// data of packet is "swwxABCDEFGHIm"

#include <Arduino.h>
#include <LORA.h>

#define PIN_NSS GPIO_NUM_10
#define PIN_NRESET GPIO_NUM_9
#define PIN_txEn GPIO_NUM_8
#define PIN_rxEn GPIO_NUM_7

LORA *lora;
unsigned char tx_buf[] = {"swwxABCDEFGHIm"};
unsigned char val;

void setup()
{
    Serial.begin(115200);

    LORA instance = LORA(PIN_NSS, PIN_NRESET, PIN_txEn, PIN_rxEn);
    *lora = instance;

    if (!lora->init())
    {
        Serial.println("Init fail!");
    }
}

void loop()
{
    val = Serial.read(); // please make sure serial is OK befor runing this code
    
    if (val == 'T')      // tx a packet if receive "T"
    {
        lora->txPacket(tx_buf, sizeof(tx_buf));
        lora->enterStbyMode(); // turn to standby mode
        Serial.println("tx");
    }
}