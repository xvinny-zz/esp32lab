/**
 * @file ConnectionManager.cpp
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief 
 * @version 0.1
 * @date 21/02/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#include "ConnectionManager.h"

void WiFiEvent(WiFiEvent_t event)
{
    // #define CONNMGR_LOGD_0001 "Evento WiFi: %d\n"
    // ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0001, event);
    
    switch (event)
    {
    case SYSTEM_EVENT_WIFI_READY:
        // #define CONNMGR_LOGD_0002 "Interface de WiFi pronta."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0002);
        break;
    case SYSTEM_EVENT_SCAN_DONE:
        // #define CONNMGR_LOGD_0003 "Escaneamento de access points completado."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0003);
        break;
    case SYSTEM_EVENT_STA_START:
        // #define CONNMGR_LOGD_0004 "Cliente de WiFi iniciado."
        //ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0004);
        break;
    case SYSTEM_EVENT_STA_STOP:
        // #define CONNMGR_LOGD_0005 "Cliente de WiFi parado."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0005);
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        // #define CONNMGR_LOGD_0006 "Conectado ao access point."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0006);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        // #define CONNMGR_LOGD_0007 "Desconectou do access point WiFi."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0007);
        break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        // #define CONNMGR_LOGD_0008 "O modo de autenticacao do access point mudou."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0008);
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        // #define CONNMGR_LOGD_0009 "IP: %s"
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0009, WiFi.localIP().toString().c_str());

        // Set up mDNS responder:
        // - first argument is the domain name, in this example
        //   the fully-qualified domain name is "esp8266.local"
        // - second argument is the IP address to advertise
        //   we send our IP address on the WiFi network
        if (!MDNS.begin("esp32")) 
        {
            // #define CONNMGR_LOGE_0001 "Erro ao configurar o MDNS responder."
            ESP_LOGE(TAG_CONNMGR, CONNMGR_LOGE_0001);
            while(1) 
            {
                delay(1000);
            }
        }
        // #define CONNMGR_LOGD_0029 "mDNS responder iniciado."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0029);
        break;
    case SYSTEM_EVENT_STA_LOST_IP:
        // #define CONNMGR_LOGD_0010 "Perdeu o endereco IP e o endereco foi resetado para 0."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0010);
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        // #define CONNMGR_LOGD_0011 "WiFi Protected Setup: sucesso no modo de entrada."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0011);
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        // #define CONNMGR_LOGD_0012 "WiFi Protected Setup: falha no modo de entrada."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0012);
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        // #define CONNMGR_LOGD_0013 "WiFi Protected Setup: timeout no modo de entrada."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0013);
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        // #define CONNMGR_LOGD_0014 "WiFi Protected Setup: solicitado pin code no modo de entrada."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0014);
        break;
    case SYSTEM_EVENT_AP_START:
        // #define CONNMGR_LOGD_0015 "Access point WiFi iniciado."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0015);
        break;
    case SYSTEM_EVENT_AP_STOP:
        // #define CONNMGR_LOGD_0016 "Access point WiFi parado."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0016);
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        // #define CONNMGR_LOGD_0017 "Cliente WiFi conectado."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0017);
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        // #define CONNMGR_LOGD_0018 "Cliente WiFi desconectado."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0018);
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        // #define CONNMGR_LOGD_0019 "Assinalou endereco IP ao Cliente WiFi."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0019);
        break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        // #define CONNMGR_LOGD_0020 "Recebido probe request."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0020);
        break;
    case SYSTEM_EVENT_GOT_IP6:
        // #define CONNMGR_LOGD_0021 "IPv6 preferido."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0021);
        break;
    case SYSTEM_EVENT_ETH_START:
        // #define CONNMGR_LOGD_0022 "Ethernet inicializada."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0022);
        break;
    case SYSTEM_EVENT_ETH_STOP:
        // #define CONNMGR_LOGD_0023 "Ethernet parada."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0023);
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        // #define CONNMGR_LOGD_0024 "Ethernet conectada."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0024);
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        // #define CONNMGR_LOGD_0025 "Ethernet desconectada."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0025);
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
        // #define CONNMGR_LOGD_0026 "Endereco de IP obtido."
        ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0026);
        break;
    default:
        break;
    }
}

ConnectionManagerClass::ConnectionManagerClass()
{
}

void ConnectionManagerClass::begin()
{
    // // delete old config
    // WiFi.disconnect(true);
    WiFi.disconnect(true);
    
    delay(1000);

    WiFi.onEvent(WiFiEvent);

    WiFi.mode(WIFI_AP_STA);
    WiFi.setAutoReconnect(true);

    // #define CONNMGR_LOGD_0027 "Inicializando a WiFi..."
    ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0027);
    WiFi.begin("qflux", "33285901ctba");
    //WiFi.begin("VidoMobi", "87654321");

    
    // #define CONNMGR_LOGD_0028 "Gerenciador de conectividade inicializado."
    ESP_LOGD(TAG_CONNMGR, CONNMGR_LOGD_0028);
}

bool ConnectionManagerClass::isConnected()
{
    return WiFi.isConnected();
}

ConnectionManagerClass ConnectionManager;