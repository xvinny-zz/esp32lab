#ifndef __MESSAGE_LIBRARY_H__
#define __MESSAGE_LIBRARY_H__

#define TAG_ENGINE "[ENGINE]"
#define TAG_GPS "[GPS]"
#define TAG_MPU "[MPU]"
#define TAG_BUTTONMANAGER "[BUTTONS]"
#define TAG_CONFMGR "[CONFIG]"
#define TAG_LOG "[LOG]"
#define TAG_NAVCOMMMGR "[NAVCOMMMGR]"

#define LOGE_0001 "Falha ao montar o SD card."
#define LOGE_0002 "Falha ao inicializar a MPU. Tentando novamente..."
#define LOGE_0003 "O arquivo de log %s nao existe"
#define LOGE_0004 "Falha ao criar o diretorio de log."
#define LOGE_0005 "Erro ao abrir o arquivo %s no modo append."
#define LOGE_0006 "Falha ao adicionar os dados de telemetria no arquivo."
#define LOGE_0007 "Falha ao realizar a calibracao."
#define LOGE_0008 "Falha ao inicializar o Engine. Reiniciando em 3s..."
#define LOGE_0009 "Erro ao abrir o arquivo %s no modo leitura."

#define LOGI_0001 "Inicializando o sistema. Versao %s"

#define LOGI_0003 "Inicializando o GPS."
#define LOGI_0004 "GPS inicializado."
#define LOGI_0005 "Taxa de amostragem configurada. [%d]"
#define LOGI_0006 "Iniciando o acelerometro."
#define LOGI_0007 "Acelerometro iniciado."  
#define LOGI_0008 "Calibrando o acelerometro."
#define LOGI_0009 "Acelerometro calibrado. Gyro offsets: %.2f|%.2f|%.2f"
#define LOGI_0010 "Inicializando o gerenciador de botoes."
#define LOGI_0011 "Gerenciador de botoes inicializado."
#define LOGI_0012 "Iniciando o NavCommMonitor."
#define LOGI_0013 "Configurando o estado do sistema: %d"
#define LOGI_0014 "Arquivo de log aberto: %s (%lu bytes livres)"
#define LOGI_0015 "Engine inicializado."
#define LOGI_0016 "Inicializando a comunicacao com o NavComm."

#define LOGD_0001 "Carregando o arquivo de log."
#define LOGD_0002 "Numero do arquivo de log: %lu"
#define LOGD_0003 "Abrindo arquivo de log %s"
#define LOGD_0004 "Inicializando o sistema de log."
#define LOGD_0005 "Criando um arquivo de log numero %lu"
#define LOGD_0006 "Logando: %s"
#define LOGD_0007 "Enviando comando isAlive."
#define LOGD_0008 "Status de Memoria Livre (Heap: %u, Psram: %u)"

#define SD_CARD_TYPE_MMC "MMC"
#define SD_CARD_TYPE_SDSC "SDSC"
#define SD_CARD_TYPE_SDHC "SDHC"
#define SD_CARD_TYPE_UNKNOWN "UNKNOWN"

#define NAMESPACE_LOG "log"
#define KEY_LOG_FILE_NUMBER "file_number"
#define LOG_FOLDER "/LOG"

#endif //__MESSAGE_LIBRARY_H__