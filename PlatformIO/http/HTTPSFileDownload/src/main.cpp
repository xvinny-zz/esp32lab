#include <Arduino.h>
#include <HTTPClient.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <WiFi.h>

const char *ssid = "qflux";
const char *password = "33285901ctba";

int download(String url);

void setup()
{
    Serial.begin(115200);

    if (!SD.begin())
    {
        Serial.println("Card Mount Failed");
        return;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("UNKNOWN");
    }

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.print("WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());

    download("https://anjostorage.blob.core.windows.net/firmwares/1/firmware.bin");
}

void loop()
{
    // put your main code here, to run repeatedly:
    delay(1000);
}

int download(String url)
{
    int downloaded = 0;
    int total = 1;
    uint8_t buff[1024] = {0};
    size_t size = sizeof(buff);
    int ret = 0;
    HTTPClient http;

    File file;

    http.begin(url);

    int httpCode = http.GET();

    if (httpCode > 0 && httpCode == HTTP_CODE_OK)
    {
        // get lenght of document (is -1 when Server sends no Content-Length header)
        int len = http.getSize();
        int percent = 0;
        total = len;
        downloaded = 0;

        // get tcp stream
        WiFiClient *stream = http.getStreamPtr();

        //write bin file to sdcard
        SD.remove("/firmware.bin");
        file = SD.open("/firmware.bin", FILE_WRITE);

        // read all data from server
        while (http.connected() && (len > 0))
        {
            // get available data size
            size = stream->available();

            if (size > 0)
            {
                // read up to 128 byte
                int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
                // write to storage
                //info.saveDataCallback(buff, c);
                file.write(buff, c);
                downloaded += c;

                percent = (100 * downloaded) / total;

                if (percent % 10 == 0)
                    Serial.printf("Downloaded = %d\n", percent);

                if (len > 0)
                {
                    len -= c;
                }
            }
            delay(1);
        }

        file.close();
    }
    else
    {
        Serial.println("[HTTP] GET... failed!");
        ret = -1;
    }

    http.end();

    if (downloaded == total)
    {
        Serial.println("Download firmware ... success!");
        ret = 0;
    }
    else
    {
        Serial.println("Download firmware ... failed!");
        ret = -1;
    }

    return ret;
}