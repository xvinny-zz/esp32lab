/**
 * @file main.cpp
 * @author Vinicius de Sa
 * @brief Este exemplo demonstra o funcionamento da SdFat.
 * @version 0.1
 * @date 04/06/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */


#include <SPI.h>
#include "SdFat.h"

#define SD_CS_PIN SS

SdFat SD;
File file;

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    ESP_LOGI("", "Inicializando o SD card...");

    if (!SD.begin(SD_CS_PIN))
    {
        ESP_LOGE("", "Falha na inicializacao. Reiniciando...");
        ESP.restart();
    }
    
    Serial.println("SD inicializado! Abrindo arquivo teste.txt para escrita...");

    file = SD.open("/teste.txt", FILE_WRITE);

    if (file)
    {
        ESP_LOGI("Escrevendo no arquivo teste.txt...");

        file.println("Teste de escrita.");
        file.close();
        
        ESP_LOGI("", "Dados escritos!");
    }
    else
    {
        ESP_LOGE("", "Falha ao abrir o arquivo teste.txt.");
    }

    file = SD.open("/teste.txt");
    String content = "";
    if (file)
    {
        while (file.available())
        {
            content += (char)file.read();
        }
        
        ESP_LOGI("", "Conteudo do arquivo: %s", content.c_str());

        file.close();
    }
    else
    {
        ESP_LOGE("", "Falha ao abrir o arquivo teste.txt.");
    }
}

void loop()
{
}