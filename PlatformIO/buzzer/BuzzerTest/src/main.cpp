#include <Arduino.h>

#define PIN_BUZZER GPIO_NUM_32

void setup() 
{
    Serial.begin(115200);

    ESP_LOGD("MAIN", "Inicializando o pino do Buzzer...");
    delay(1000);

    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
    
    ESP_LOGD("MAIN", "Pino do Buzzer inicializado! Iniciando testes...");
    delay(1000);
}

void loop() 
{
    ESP_LOGD("MAIN", "Pino Buzzer = ALTO");
    digitalWrite(PIN_BUZZER, HIGH);
    delay(2000);
    
    ESP_LOGD("MAIN", "Pino Buzzer = BAIXO");
    digitalWrite(PIN_BUZZER, LOW);
    delay(2000);
}