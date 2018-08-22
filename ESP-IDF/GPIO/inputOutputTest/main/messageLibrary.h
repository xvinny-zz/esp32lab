/*
 * messageLibrary.h
 *
 *  Created on: 16 de mar de 2018
 *      Author: Vinicius
 */

#ifndef MAIN_MESSAGELIBRARY_H_
#define MAIN_MESSAGELIBRARY_H_

//////////////////////////////
//			FILES			//
//////////////////////////////
#define SDCARD_BASE_PATH "/sdcard"
#define LOG_FILE_NAME_GENERAL "/sdcard/general.log"
#define COUNTERS_FILE_NAME "/sdcard/counters.dat"
#define SETTINGS_FILE_NAME "/sdcard/settings.dat"
#define MESSAGE_LOG_FILE_NAME "/sdcard/%04d%02d%02d.log"

//////////////////////////////////
//			 MESSAGES			//
//////////////////////////////////
#define MSG_AZURE_STARTED "$001,01,System Restarted\r\n"
#define MSG_AZURE_STARTING_PROCESS "$001,02,Starting process\r\n"

#define MSG_GPS_INITIALIZE_ERROR "$002,99,Error initializing GPS\r\n"
#define MSG_GPS_INITIALIZE_SUCCESS "$002,01,GPS initialize success\r\n"

#define MSG_CONNMGR_INITIALIZING "$003,01,Initializing connection manager\r\n"

#define MSG_CONNMGR_VERIFYING_NETWORKS "$004,01,Verifying networks\r\n"
#define MSG_CONNMGR_START_WIFI_SCAN "$004,02,Starting Wi-Fi scan\r\n"
#define MSG_CONNMGR_CHANGING_GSM_TO_WIFI "$004,03,Changing from GSM to Wi-Fi\r\n"
#define MSG_CONNMGR_WIFI_INITIALIZING "$004,04,01,Initializing Wi-Fi\r\n"
#define MSG_CONNMGR_WIFI_INITIALIZED "$004,04,02,Wi-Fi initialized\r\n"
#define MSG_CONNMGR_WIFI_NETWORK_FOUND "$004,04,03,Network found\r\n"
#define MSG_CONNMGR_WIFI_NETWORK_FOUND_WEAK_SIGNAL "$004,04,04,Wi-Fi initialized\r\n"
#define MSG_CONNMGR_GSM_CONNECTING "$004,05,01,Connecting GSM\r\n"
#define MSG_CONNMGR_GSM_CONNECTION_ERROR "$004,05,99,Error connecting GSM\r\n"
#define MSG_CONNMGR_GSM_CONNECTED "$004,05,02,Connected GSM\r\n"

#define MSG_COUNTERS_READING "$005,01,Reading counters\r\n"
#define MSG_COUNTERS_READ "$005,02,Counters read\r\n"
#define MSG_COUNTERS_SEP_MISSING "$005,99,01,Counters separator missing\r\n"
#define MSG_COUNTERS_NOT_LOADED "$005,99,02,Counters not loaded\r\n"
#define MSG_COUNTERS_NOT_FOUND "$005,99,03,Counters not found\r\n"

#define MSG_SERIALIZER_INITIALIZE_ERROR "$006,99,Error initializing serializer\r\n"

#define MSG_SETTINGS_NOT_FOUND "$007,99,Settings not found, restarting\r\n"
#define MSG_SETTINGS_READING "$007,01,Reading settings\r\n"
#define MSG_SETTINGS_READ "$007,02,Settings read\r\n"

#define MSG_AZURE_SEND_MESSAGE_TIMEOUT "$008,01,99,Send message timeout\r\n"
#define MSG_AZURE_TRIP_INFO_NOT_CREATED "$008,02,99,Trip info not created\r\n"

#define MSG_MPU6050_INITIALIZE_ERROR "$009,01,99,MPU6050 failed to initialize\r\n"
#define MSG_MPU6050_INITIALIZE_SUCCESS "$009,01,01,MPU6050 initialized with success\r\n"


//////////////////////////////
//		 LOG INFOS			//
//////////////////////////////
#define LOGI_0001 "GPS inicializado"
#define LOGI_0002 "Finalizando o handle Azure..."
#define LOGI_0003 "Azure finalizado!"
#define LOGI_0004 "Inicializando o handle Azure..."
#define LOGI_0005 "Azure inicializado!"
#define LOGI_0006 "GPS: [%02d/%02d/%04d %02d:%02d:%02d] Fix: %d | HDOP: %.2f | Sat: %d | ExtV: %.2f | IntV: %.2f"
#define LOGI_0007 "Mov. realizado: %.2f metros | Velocidade: %.2f | Odometro: %lld"
#define LOGI_0008 "Mov. filtrado: %.2f metros | Velocidade: %.2f"
#define LOGI_0009 "Transmitindo: %s\n"
#define LOGI_0010 "Iniciando processamento..."
#define LOGI_0011 "Ult. msg enviada ha %llu segundos"
#define LOGI_0012 "Engine off. ExtV: %.2f | IntV: %.2f"
#define LOGI_0013 "Conectando 3G. APN: %s | User: %s | Pwd: %s"
#define LOGI_0014 "Conectando a WiFi."
#define LOGI_0015 "Desconectando o 3G."
#define LOGI_0016 "Buscando rede %s. APs encontrados: %d"
#define LOGI_0017 "WIFI Station iniciada!"
#define LOGI_0018 "WiFi conectada!"
#define LOGI_0019 "Conectado na WiFi. IP: %s"
#define LOGI_0020 "Desconectado da rede WiFi."
#define LOGI_0021 ""	// MSG VAGA
#define LOGI_0022 "Atualizando arquivo de contadores: Horimetro: %lld | Odometro: %lld"
#define LOGI_0023 "Mensagem [%d] enviada!"
#define LOGI_0024 "Aguardando confirmação do recebimento..."
#define LOGI_0025 "ChipId inicializado. Valor: %lld"
#define LOGI_0026 "Arquivo de configurações: [%s]"
#define LOGI_0027 "Arquivo de contadores: [%s]"
#define LOGI_0028 "Contadores carregados: Horimetro: %lld | Odometro: %lld"
#define LOGI_0029 "IoTHub: Mensagem recebida [%d]: ID: %s | Correlation ID: %s | Tipo Conteudo: %s | Encoding: %s | Dados: <<<%.*s>>> & Tamanho=%d"
#define LOGI_0030 "IoTHub: Propriedades da mensagem: "
#define LOGI_0031 "IoTHub: \tPar: %s Valor: %s"
#define LOGI_0032 "IoTHub: Recebimento confirmado! MessageTrackingId = %zu | Result = %s"
#define LOGI_0033 "IotHub: Status de envio da mensagem: [%s]"
#define LOGI_0034 "Lendo o arquivo de configuracoes."
#define LOGI_0035 "Arquivo de configuracoes carregado com sucesso."
#define LOGI_0036 "Arquivo de contadores nao encontrado. Um novo arquivo foi criado."
#define LOGI_0037 "Iniciando transmissao (serializando mensagem)."
#define LOGI_0038 "Inicializando o SD card."
#define LOGI_0039 "SD card inicializado."
#define LOGI_0040 "Desmontando a particao e desabilitando o SD card."
#define LOGI_0041 "Lendo arquivo %s (tamanho: %ld bytes)."
#define LOGI_0042 "Inicializando o GPS."
#define LOGI_0043 "Instalando o driver da serial do GPS."
#define LOGI_0044 "GPS inicializado."
#define LOGI_0045 "Inicializando a MPU."
#define LOGI_0046 "MPU inicializada."
#define LOGI_0047 "Inicializando o controlador da GPIO."
#define LOGI_0048 "GPIO inicializado."
#define LOGI_0049 "Inicializando a comunicacao I2C."
#define LOGI_0050 "Ligando o modulo."
#define LOGI_0051 "Configurando a amostragem."
#define LOGI_0052 "Configurando parametros gerais."
#define LOGI_0053 "Configurando o giroscopio."
#define LOGI_0054 "Configurando o acelerometro."
#define LOGI_0055 "Inicializando o Gerenciador de Leds."
#define LOGI_0056 "Executando auto-teste dos leds."
#define LOGI_0057 "Ligando todos os leds."
#define LOGI_0058 "Desligando todos os leds."

//////////////////////////////
//		 LOG ERRORS			//
//////////////////////////////
#define LOGE_0001 "Falha ao inicializar o GPS. Reiniciando em 5s."
#define LOGE_0002 "Ocorreu timeout no envio desta mensagem. Verifique se há conectividade. Timeout: %lld"
#define LOGE_0003 "Falha ao criar o TrackerTripInfo. Reiniciando em 5 segundos..."
#define LOGE_0004 "Erro ao carregar os contadores. Separador do arquivo counters nao encontrado."
#define LOGE_0005 "Erro ao definir a configuracao da WiFi."
#define LOGE_0006 "Nao foi possivel conectar GSM. Reiniciando..."
#define LOGE_0007 "Erro ao criar a mensagem para envio ao IoTHub."
#define LOGE_0008 "Falha ao enviar a mensagem [%d]. Erro: %s"
#define LOGE_0009 "Status de conectividade invalido."
#define LOGE_0010 "IoTHub: Nao foi possivel obter os dados da mensagem."
#define LOGE_0011 "Arquivo de configuracoes nao encontrado."
#define LOGE_0012 "Falha ao montar o sistema de arquivos. Se voce deseja que o cartao seja formatado, ajuste format_if_mount_failed = true."
#define LOGE_0013 "Falha ao inicializar o cartao (%d). Verifique se as conexoes do SD card possuem resistores pull-up."
#define LOGE_0014 "Falha ao abrir o arquivo %s para escrita."
#define LOGE_0015 "Falha ao abrir o arquivo %s para append."
#define LOGE_0016 "Falha ao abrir o arquivo %s para leitura."
#define LOGE_0017 "Nao foi possivel transmitir a mensagem de inicio de viagem."
#define LOGE_0018 "Nao foi possivel transmitir os dados de viagem."
#define LOGE_0019 "Nao foi possivel transmitir o encerramento da viagem."
#define LOGE_0020 "Falha ao configurar os parametros do GPS."
#define LOGE_0021 "Falha ao configurar os pinos do GPS."
#define LOGE_0022 "Falha ao instalar o driver da serial do GPS."
#define LOGE_0023 "Falha ao inicializar a MPU. Erro: %d. Reiniciando em 5s."
#define LOGE_0024 "Falha ao inicializar o serialiador. Reiniciando em 5s."

//////////////////////////////
//			TAGS			//
//////////////////////////////
#define TAG_AZURE "[AZURE]"
#define TAG_CONNECTION_MANAGER "[CONNMGR]"
#define TAG_SDCARD "[SDCARD]"
#define TAG_GPS "[GPS]"
#define TAG_MPU "[MPU]"
#define TAG_GPIO "[GPIO]"
#define TAG_LEDHANDLER "[LED]"

#endif /* MAIN_MESSAGELIBRARY_H_ */
