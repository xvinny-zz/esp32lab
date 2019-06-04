#include <Arduino.h>
#include "TrackerWrapper.h"

String introduction = "Teste do GSM\n \
                      Comandos: \n \
                      -> \"CONNECT\": Conectar\n \
                      -> \"DISCONNECT\": Desconectar\n \
                      -> \"ISCONNECTED\": Esta conectado?\n \
                      -> \"POSITION\": Enviar posicao padrao\n \
                      -> \"LOGIN\": Enviar o comando de login\n \
                      -> \"HEARTBEAT\": Enviar o comando de heartbeat\n \
                      -> \"GETDEVICES\": Obter os dispositivos cadastrados\n \
                      -> \"POSITION:<ID>,<LAT>,<LON>,<BAT>\": Esta conectado?\n";

void setup()
{
    Serial.begin(115200);

    TrackerWrapper.begin();
    ESP_LOGI("TRACKER", "%s", introduction.c_str());
}

void loop()
{
    String text = Serial.readStringUntil('\n');

    if (text.length() > 0)
    {
        if (text.compareTo("CONNECT") == 0)
        {
            ESP_LOGI("TRACKER", "Enviando comando CONNECT");
            if (TrackerWrapper.queueConnect() != ESP_OK)
                ESP_LOGE("TRACKER", "Nao eh possivel enviar o comando CONNECT: ja conectado.");
        }
        else if (text.compareTo("DISCONNECT") == 0)
        {
            ESP_LOGI("TRACKER", "Enviando comando DISCONNECT");
            if (TrackerWrapper.queueDisconnect() != ESP_OK)
                ESP_LOGE("TRACKER", "Nao eh possivel enviar o comando DISCONNECT: ja desconectado.");
        }
        else if (text.compareTo("ISCONNECTED") == 0)
        {
            bool isConnected = TrackerWrapper.isConnected();
            ESP_LOGI("TRACKER", "Esta conectado? %s", isConnected ? "Sim" : "Nao");
        }
        else if (text.compareTo("POSITION") == 0)
        {
            ESP_LOGI("TRACKER", "Enviando comando POSITION");
            if (TrackerWrapper.queuePosition() != ESP_OK)
                ESP_LOGE("TRACKER", "Nao eh possivel enviar o comando POSITION.");
        }
        else if (text.compareTo("LOGIN") == 0)
        {
            ESP_LOGI("TRACKER", "Enviando comando LOGIN");
            if (TrackerWrapper.queueLogin() != ESP_OK)
                ESP_LOGE("TRACKER", "Nao eh possivel enviar o comando LOGIN.");
        }
        else if (text.compareTo("HEARTBEAT") == 0)
        {
            ESP_LOGI("TRACKER", "Enviando comando HEARTBEAT");
            if (TrackerWrapper.queueHeartbeat() != ESP_OK)
                ESP_LOGE("TRACKER", "Nao eh possivel enviar o comando HEARTBEAT.");
        }
        else if (text.compareTo("GETDEVICES") == 0)
        {
            ESP_LOGI("TRACKER", "Enviando comando GETDEVICES");
            if (TrackerWrapper.queueGetDevices() != ESP_OK)
                ESP_LOGE("TRACKER", "Nao eh possivel enviar o comando GETDEVICES.");
        }
        else
        {
            if (text.startsWith(String("POSITION:")))
            {
                text.remove(0, (text.indexOf(':') + 1));
                if (text.length() <= 0)
                    ESP_LOGE("MAIN", "Comando invalido");
                else
                {
                    String id = text.substring(0, text.indexOf(','));
                    text.remove(0, (text.indexOf(',') + 1));
                    if (text.length() <= 0)
                        ESP_LOGE("MAIN", "Comando invalido");
                    else
                    {
                        double latitude = atof(text.substring(0, text.indexOf(',')).c_str());
                        text.remove(0, (text.indexOf(',') + 1));
                        if (text.length() <= 0)
                            ESP_LOGE("MAIN", "Comando invalido");
                        else
                        {
                            double longitude = atof(text.substring(0, text.indexOf(',')).c_str());
                            text.remove(0, (text.indexOf(',') + 1));
                            if (text.length() <= 0)
                                ESP_LOGE("MAIN", "Comando invalido");
                            else
                            {
                                int battery = atoi(text.c_str());
                                ESP_LOGD("MAIN", "Solicitando posicao: Id: %s, Lat: %.2f, Lon: %.2f, Bat: %d",
                                         id.c_str(), latitude, longitude, battery);
                                
                                //TrackerWrapper.postPosition(id, latitude, longitude, battery);
                            }
                        }
                    }
                }
            }
            else
                ESP_LOGI("TRACKER", "Comando nao reconhecido. Texto: %s", text.c_str());
        }
    }

    delay(10);
}