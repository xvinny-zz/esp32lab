// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>

#include "iothub_client.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "iothubtransportmqtt.h"
#include "iothub_client_options.h"

#ifdef MBED_BUILD_TIMESTAMP
#define SET_TRUSTED_CERT_IN_SAMPLES
#endif // MBED_BUILD_TIMESTAMP

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
#include "certs.h"
#endif // SET_TRUSTED_CERT_IN_SAMPLES

/*String containing Hostname, Device Id & Device Key in the format:                         */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
//static const char* connectionString = "HostName=ViniTestHub.azure-devices.net;DeviceId=0001;SharedAccessKey=zP57Q1etwCxBP04Hqij+F0sCgYwnvioqzSA6lWQFJpM=";
static const char* connectionString = "HostName=himalaia-iot-hub.azure-devices.net;DeviceId=0001;SharedAccessKey=T7OzyKoEXyfSGXmxmNY68XcNIASN7cfILfzTB904XFs=";

static int callbackCounter;
static char msgText[1024];
static char propText[1024];
static bool g_continueRunning;
#define MESSAGE_COUNT 5
#define DOWORK_LOOP_NUM     3


typedef struct EVENT_INSTANCE_TAG
{
    IOTHUB_MESSAGE_HANDLE messageHandle;
    size_t messageTrackingId;  // For tracking the messages within the user callback.
} EVENT_INSTANCE;

/*
 * Função: ReceiveMessageCallback
 * Objetivo: Callback para recebimento de mensagens.
 * Parâmetros:
 * 		IOTHUB_MESSAGE_HANDLE message: handle da mensagem recebida.
 * 		void* userContextCallback: conteúdo da mensagem recebida.
 * Retorno: enumeração com os seguintes valores:
 * 		IOTHUBMESSAGE_ACCEPTED: Mensagem aceita
 * 		IOTHUBMESSAGE_REJECTED: Mensagem rejeitada
 * 		IOTHUBMESSAGE_ABANDONED: Mensagem abandonada
 * */
static IOTHUBMESSAGE_DISPOSITION_RESULT ReceiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
    int* counter = (int*)userContextCallback;
    const char* buffer;
    size_t size;
    MAP_HANDLE mapProperties;
    const char* messageId;
    const char* correlationId;
    const char* userDefinedContentType;
    const char* userDefinedContentEncoding;

    // Message properties
    if ((messageId = IoTHubMessage_GetMessageId(message)) == NULL)
    {
        messageId = "<null>";
    }

    if ((correlationId = IoTHubMessage_GetCorrelationId(message)) == NULL)
    {
        correlationId = "<null>";
    }

    if ((userDefinedContentType = IoTHubMessage_GetContentTypeSystemProperty(message)) == NULL)
    {
        userDefinedContentType = "<null>";
    }

    if ((userDefinedContentEncoding = IoTHubMessage_GetContentEncodingSystemProperty(message)) == NULL)
    {
        userDefinedContentEncoding = "<null>";
    }

    // Message content
    if (IoTHubMessage_GetByteArray(message, (const unsigned char**)&buffer, &size) != IOTHUB_MESSAGE_OK)
    {
        (void)printf("unable to retrieve the message data\n");
    }
    else
    {
        (void)printf("Received Message [%d]\n Message ID: %s\n Correlation ID: %s\n Content-Type: %s\n Content-Encoding: %s\n Data: <<<%.*s>>> & Size=%d\n",
            *counter, messageId, correlationId, userDefinedContentType, userDefinedContentEncoding, (int)size, buffer, (int)size);
        // If we receive the work 'quit' then we stop running
        if (size == (strlen("quit") * sizeof(char)) && memcmp(buffer, "quit", size) == 0)
        {
            g_continueRunning = false;
        }
    }

    // Retrieve properties from the message
    mapProperties = IoTHubMessage_Properties(message);
    if (mapProperties != NULL)
    {
        const char*const* keys;
        const char*const* values;
        size_t propertyCount = 0;
        if (Map_GetInternals(mapProperties, &keys, &values, &propertyCount) == MAP_OK)
        {
            if (propertyCount > 0)
            {
                size_t index;

                printf(" Message Properties:\n");
                for (index = 0; index < propertyCount; index++)
                {
                    (void)printf("\tKey: %s Value: %s\n", keys[index], values[index]);
                }
                (void)printf("\n");
            }
        }
    }

    /* Some device specific action code goes here... */
    (*counter)++;
    return IOTHUBMESSAGE_ACCEPTED;
}

/*
 * Função: SendConfirmationCallback
 * Objetivo: Callback acionado quando ocorre a confirmação de envio da mensagem.
 * Parâmetros:
 * 		IOTHUB_CLIENT_CONFIRMATION_RESULT result: enumeração com o resultado do envio.
 * 		void* userContextCallback: conteúdo da mensagem enviada.
 * Retorno: vazio
 * */
static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    EVENT_INSTANCE* eventInstance = (EVENT_INSTANCE*)userContextCallback;
    (void)printf("Confirmation[%d] received for message tracking id = %zu with result = %s\n", callbackCounter, eventInstance->messageTrackingId, ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result));
    /* Some device specific action code goes here... */
    callbackCounter++;
    IoTHubMessage_Destroy(eventInstance->messageHandle);
}

/*
 * Função: iothub_client_sample_mqtt_run
 * Objetivo: Função para execução do exemplo do IoT Hub.
 * Parâmetros: nenhum
 * Retorno: vazio
 * */
void iothub_client_sample_mqtt_run(void)
{
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

    EVENT_INSTANCE messages[MESSAGE_COUNT];

    g_continueRunning = true;
    srand((unsigned int)time(NULL));
    double avgWindSpeed = 10.0;
    double minTemperature = 20.0;
    double minHumidity = 60.0;
    
    callbackCounter = 0;
    int receiveContext = 0;

    if (platform_init() != 0)
    {
        (void)printf("Failed to initialize the platform.\n");
    }
    else
    {
        if ((iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol)) == NULL)
        {
            (void)printf("ERROR: iotHubClientHandle is NULL!\n");
        }
        else
        {
            bool traceOn = true;
            IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_LOG_TRACE, &traceOn);

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
            // For mbed add the certificate information
            if (IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_TRUSTED_CERT, certificates) != IOTHUB_CLIENT_OK)
            {
                printf("failure to set option \"TrustedCerts\"\n");
            }
#endif // SET_TRUSTED_CERT_IN_SAMPLES

            /* Setting Message call back, so we can receive Commands. */
            if (IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, ReceiveMessageCallback, &receiveContext) != IOTHUB_CLIENT_OK)
            {
                (void)printf("ERROR: IoTHubClient_LL_SetMessageCallback..........FAILED!\n");
            }
            else
            {
                (void)printf("IoTHubClient_LL_SetMessageCallback...successful.\n");

                /* Now that we are ready to receive commands, let's send some messages */
                size_t iterator = 0;
                double temperature = 0;
                double humidity = 0;
                do
                {
                    if (iterator < MESSAGE_COUNT)
                    {
                        temperature = minTemperature + (rand() % 10);
                        humidity = minHumidity +  (rand() % 20);
                        sprintf_s(msgText, sizeof(msgText), "{\"deviceId\":\"myFirstDevice\",\"windSpeed\":%.2f,\"temperature\":%.2f,\"humidity\":%.2f}", avgWindSpeed + (rand() % 4 + 2), temperature, humidity);
                        if ((messages[iterator].messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char*)msgText, strlen(msgText))) == NULL)
                        {
                            (void)printf("ERROR: iotHubMessageHandle is NULL!\n");
                        }
                        else
                        {

                            (void)IoTHubMessage_SetMessageId(messages[iterator].messageHandle, "MSG_ID");
                            (void)IoTHubMessage_SetCorrelationId(messages[iterator].messageHandle, "CORE_ID");
                            (void)IoTHubMessage_SetContentTypeSystemProperty(messages[iterator].messageHandle, "application%2Fjson");
                            (void)IoTHubMessage_SetContentEncodingSystemProperty(messages[iterator].messageHandle, "utf-8");

                            messages[iterator].messageTrackingId = iterator;
                            MAP_HANDLE propMap = IoTHubMessage_Properties(messages[iterator].messageHandle);
                            (void)sprintf_s(propText, sizeof(propText), temperature > 28 ? "true" : "false");
                            if (Map_AddOrUpdate(propMap, "temperatureAlert", propText) != MAP_OK)
                            {
                                (void)printf("ERROR: Map_AddOrUpdate Failed!\n");
                            }

                            if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messages[iterator].messageHandle, SendConfirmationCallback, &messages[iterator]) != IOTHUB_CLIENT_OK)
                            {
                                (void)printf("ERROR: IoTHubClient_LL_SendEventAsync..........FAILED!\n");
                            }
                            else
                            {
                                (void)printf("IoTHubClient_LL_SendEventAsync accepted message [%d] for transmission to IoT Hub.\n", (int)iterator);
                            }
                        }

                    }
                    IoTHubClient_LL_DoWork(iotHubClientHandle);
                    ThreadAPI_Sleep(1);

                    iterator++;
                } while (g_continueRunning);

                (void)printf("iothub_client_sample_mqtt has gotten quit message, call DoWork %d more time to complete final sending...\n", DOWORK_LOOP_NUM);
                size_t index = 0;
                for (index = 0; index < DOWORK_LOOP_NUM; index++)
                {
                    IoTHubClient_LL_DoWork(iotHubClientHandle);
                    ThreadAPI_Sleep(1);
                }
            }
            IoTHubClient_LL_Destroy(iotHubClientHandle);
        }
        platform_deinit();
    }
}

int main(void)
{
    iothub_client_sample_mqtt_run();
    return 0;
}
