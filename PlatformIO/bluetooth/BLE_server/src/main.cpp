#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define TAG "BLE_SERVER"
BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;
BLEAdvertising *pAdvertising;
std::map<uint16_t, conn_status_t> peerDevices;

void setup()
{
    Serial.begin(115200);
    ESP_LOGD(TAG, "Iniciando o exemplo BLE_SERVER");

    BLEDevice::init("ESP32 BLE Server");
    pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic->setValue("This message has about 32 bytes!");
    pService->start();
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    ESP_LOGD(TAG, "Caracteristica definida! Agora vc pode ler ela no teu telefone!");
}

void loop()
{
    ESP_LOGI(TAG, "Conectados: %d | ConnId: %04X", pServer->getConnectedCount(), pServer->getConnId());
    peerDevices = pServer->getPeerDevices(true);
    std::map<uint16_t, conn_status_t>::iterator it;
    for (it = peerDevices.begin(); it != peerDevices.end(); it++)
        ESP_LOGD(TAG, "Device ID: [%d] - Conectado? [%d] MTU: [%d]", it->first, it->second.connected, it->second.mtu);
    
    delay(2000);
}