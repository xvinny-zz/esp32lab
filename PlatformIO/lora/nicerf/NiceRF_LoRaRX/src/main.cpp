#include <Arduino.h>
// lora_rx.pde

// Caution:lora can only work under 3.3V
// please make sure the supply of you board is under 3.3V
// 5v supply will destroy lora module!!

// This code runs in rx mode and  works with lora_tx.pde
// Flow:receive packet from tx->print to serial
// data of packet is "swwxABCDEFGHIm"
#include <Arduino.h>
#include <LORA.h>

#define PIN_NSS GPIO_NUM_10
#define PIN_NRESET GPIO_NUM_9
#define PIN_txEn GPIO_NUM_8
#define PIN_rxEn GPIO_NUM_7

LORA *lora;

unsigned char flag = 1; //  flag of rx mode
unsigned char rx_len;
unsigned char rx_buf[20];

void setup()
{
    Serial.begin(115200);

    LORA instance = LORA(PIN_NSS, PIN_NRESET, PIN_txEn, PIN_rxEn);
    *lora = instance;

    if (!lora->init())
    {
        Serial.println("Init fail!");
    }
    
    lora->rxInit(); // wait for packet from master
}

void loop()
{
    if (flag == 1)
    {
        if (lora->waitIrq(LR_RXDONE_MASK)) // wait for RXDONE interrupt
        {
            lora->clrInterrupt();
            rx_len = lora->rxPacket(rx_buf); // read rx data
            Serial.write(rx_buf, rx_len);   // print out by serial
            Serial.println();
            lora->rxInit(); // wait for packet from master
        }
    }
}