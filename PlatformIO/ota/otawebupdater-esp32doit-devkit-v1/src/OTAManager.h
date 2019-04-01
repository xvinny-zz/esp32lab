/**
 * @file OTAManager.h
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief Gerenciador de atualizações OTA.
 * @version 0.1
 * @date 21/02/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#pragma once

#include <Update.h>
#include <WebServer.h>
#include "ConnectionManager.h"

#define TAG_OTA "[OTA]"
#define OTA_LOGD_0001 "Iniciando atualizacao em %s"
#define OTA_LOGD_0002 "Fim da atualizacao."
#define OTA_LOGD_0003 "Progresso: %u%%"
#define OTA_LOGD_0004 "Erro[%u]: %s"
#define OTA_LOGD_0005 "Inicializando gerenciador OTA."
#define OTA_LOGD_0006 "Gerenciador OTA inicializado."

class OTAManagerClass
{
    public:
        OTAManagerClass();
        void begin();
};

extern OTAManagerClass OTAManager;