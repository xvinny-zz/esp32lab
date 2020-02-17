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
#include <SdFat.h>

#define SD_CS_PIN SS
#define SDSPEED SD_SCK_MHZ(20) // result: 101 tracks in 771 ms

SdFat SD;
File file;

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    ESP_LOGI("", "Inicializando o SD card...");

    if (!SD.begin(SD_CS_PIN, SDSPEED))
    {
        ESP_LOGE("", "Falha na inicializacao. Verifique as conexoes...");
        while(1);
    }
}

void loop()
{
    ESP_LOGI("", "[%lu] SD inicializado! Abrindo arquivo teste.txt para escrita...", millis());

    file = SD.open("/teste.txt", FILE_WRITE);

    if (file)
    {
        ESP_LOGI("", "[%lu] Escrevendo no arquivo teste.txt...", millis());

        file.println("Teste de escrita.");
        file.printf("Data: %lu", millis());
        file.close();
        
        ESP_LOGI("", "[%lu] Dados escritos!", millis());
    }
    else
    {
        ESP_LOGE("", "[%lu] Falha ao escrever o arquivo teste.txt.", millis());
        while(1);
    }

    file = SD.open("/teste.txt");
    String content = "";
    if (file)
    {
        while (file.available())
        {
            content += (char)file.read();
        }
        
        ESP_LOGI("", "[%lu] Conteudo do arquivo: %s", millis(), content.c_str());

        file.close();
    }
    else
    {
        ESP_LOGE("", "[%lu] Falha ao ler o arquivo teste.txt.", millis());
        while(1);
    }

    delay(10);
}