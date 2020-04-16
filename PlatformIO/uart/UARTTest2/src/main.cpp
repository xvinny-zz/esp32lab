#include <Arduino.h>
// new comment 
#define GSM_TX 13
#define GSM_RX 14

#define SerialMon Serial
#define SerialAT Serial1

void setup() 
{
    SerialMon.begin(115200);
    SerialAT.begin(115200, SERIAL_8N1, GSM_TX, GSM_RX, false);
    delay(500);

}

esp_err_t power()
{
    gpio_num_t powerPin = GPIO_NUM_15;

    Serial.println("Iniciando procedimento de power...");

    // subir o pino POWER por mais de 1 segundo e baixar ela de volta
    pinMode(powerPin, OUTPUT);
    digitalWrite(powerPin, LOW);
    delay(250);

    SerialAT.readString();

    Serial.printf( "[%lu] Subindo nivel da GPIO %d...\n", millis(), powerPin);
    digitalWrite(powerPin, HIGH);
    delay(1200);

    Serial.printf( "[%lu] Baixando nivel da GPIO %d...\n", millis(), powerPin);
    digitalWrite(powerPin, LOW);
    delay(2000);

    Serial.println( "Procedimento de power finalizado...");

    return ESP_OK;
}

void loop() 
{
    if (SerialMon.available() > 0)
    {
        SerialMon.println("Enviando comando: ");
        
        uint8_t currentByte = 0;
        int bytesRead = 0;

        while(SerialMon.available() > 0)
        {
            currentByte = (uint8_t) SerialMon.read();
            if (bytesRead < 1 && currentByte == 0x30)
            {
                power();
                break;
            }
            else
            {
                SerialAT.write(currentByte);
                SerialMon.print((char)currentByte);
            }
            bytesRead++;
        }
    }

    if (SerialAT.available() > 0)
    {
        SerialMon.println("Lendo resposta: ");

        uint8_t currentByte = 0;

        while(SerialAT.available() > 0)
        {
            currentByte = (uint8_t) SerialAT.read();
            SerialMon.print((char)currentByte);
        }
    }

    delay(10);
}