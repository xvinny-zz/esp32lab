/**
 * @file ConnectionManager.h
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief 
 * @version 0.1
 * @date 21/02/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFiClient.h>

#define TAG_CONNMGR "[CONNMGR]"
#define CONNMGR_LOGD_0001 "Evento WiFi: %d\n"
#define CONNMGR_LOGD_0002 "Interface de WiFi pronta."
#define CONNMGR_LOGD_0003 "Escaneamento de access points completado."
#define CONNMGR_LOGD_0004 "Cliente de WiFi iniciado."
#define CONNMGR_LOGD_0005 "Cliente de WiFi parado."
#define CONNMGR_LOGD_0006 "Conectado ao access point."
#define CONNMGR_LOGD_0007 "Desconectou do access point WiFi."
#define CONNMGR_LOGD_0008 "O modo de autenticacao do access point mudou."
#define CONNMGR_LOGD_0009 "IP: %s"
#define CONNMGR_LOGD_0010 "Perdeu o endereco IP e o endereco foi resetado para 0."
#define CONNMGR_LOGD_0011 "WiFi Protected Setup: sucesso no modo de entrada."
#define CONNMGR_LOGD_0012 "WiFi Protected Setup: falha no modo de entrada."
#define CONNMGR_LOGD_0013 "WiFi Protected Setup: timeout no modo de entrada."
#define CONNMGR_LOGD_0014 "WiFi Protected Setup: solicitado pin code no modo de entrada."
#define CONNMGR_LOGD_0015 "Access point WiFi iniciado."
#define CONNMGR_LOGD_0016 "Access point WiFi parado."
#define CONNMGR_LOGD_0017 "Cliente WiFi conectado."
#define CONNMGR_LOGD_0018 "Cliente WiFi desconectado."
#define CONNMGR_LOGD_0019 "Assinalou endereco IP ao Cliente WiFi."
#define CONNMGR_LOGD_0020 "Recebido probe request."
#define CONNMGR_LOGD_0021 "IPv6 preferido."
#define CONNMGR_LOGD_0022 "Ethernet inicializada."
#define CONNMGR_LOGD_0023 "Ethernet parada."
#define CONNMGR_LOGD_0024 "Ethernet conectada."
#define CONNMGR_LOGD_0025 "Ethernet desconectada."
#define CONNMGR_LOGD_0026 "Endereco de IP obtido."
#define CONNMGR_LOGD_0027 "Inicializando a WiFi..."
#define CONNMGR_LOGD_0028 "Gerenciador de conectividade inicializado."
#define CONNMGR_LOGD_0029 "mDNS responder iniciado."

#define CONNMGR_LOGE_0001 "Erro ao configurar o MDNS responder."

class ConnectionManagerClass
{
    public:
        ConnectionManagerClass();
        void begin();
        bool isConnected();
    private:
};

extern ConnectionManagerClass ConnectionManager;