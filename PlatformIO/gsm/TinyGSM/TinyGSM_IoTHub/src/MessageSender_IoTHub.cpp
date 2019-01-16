#include "MessageSender_IoTHub.h"

static const char* connectionString = "HostName=AnjoTestHub.azure-devices.net;DeviceId=0001;SharedAccessKey=zsEa17JV8ZEOZV3jy5VlphNsEz2PPsbRARejo5m/NaA=";
TaskHandle_t * messageMonitorTaskHandle;

void ConnectIoTHub()
{
    ESP_LOGI("[MS]", "Conectando o IoTHub...");

    do
    {
        if (ConnectionManager.isConnected() == true)
        {
            if ( Esp32MQTTClient_Init((const uint8_t*)connectionString, true, true) == true )
            {
                ESP_LOGI("[MS]", "Conectado ao IoTHub com sucesso.");
                break;
            }
        }

        ESP_LOGI("[MS]", "Aguardando para tentar conectar ao IoTHub novamente...");
        delay(1000);
        
    }while(true);
}

void MonitorMessages(void * parameters)
{
    ESP_LOGI("[MS]", "Iniciando o monitor de mensagens.");

    ConnectIoTHub();

    while(1)
    {
        ESP_LOGI("[MS]", "Aguardando mensagens...");
        delay(1000);
    }
}

MessageSenderIoTHubClass::MessageSenderIoTHubClass()
{
}

void MessageSenderIoTHubClass::begin()
{
    if ( Esp32MQTTClient_SetOption( OPTION_MINI_SOLUTION_NAME, "TinyGSM_IoTHub" ) == false )
    {
        ESP_LOGE("[MS]", "Falha ao setar a option solution name");
    }

    m_smallMessageQueue = xQueueCreate( IOT_SMALL_QUEUE_LENGTH, IOT_SMALL_QUEUE_ITEM_SIZE );
    m_bigMessageQueue = xQueueCreate( IOT_BIG_QUEUE_LENGTH, IOT_BIG_QUEUE_ITEM_SIZE );
    xTaskCreate( MonitorMessages, "MonitorMessages", 10240, NULL, 1, messageMonitorTaskHandle );
}

void MessageSenderIoTHubClass::enqueueSmall( char * message , size_t size )
{

}

void MessageSenderIoTHubClass::enqueueBig( char * message , size_t size )
{

}

MessageSenderIoTHubClass MessageSenderIoTHub;