#ifndef __MESSAGE_SENDER_IOT_HUB_H__
#define __MESSAGE_SENDER_IOT_HUB_H__

#include "AzureIotHub.h"
#include "Esp32MQTTClient.h"
#include "esp_log.h"
#include "ConnectionManager.h"
#include "MessageBook.h"

const short IOT_SMALL_QUEUE_LENGTH = 10;
const short IOT_SMALL_QUEUE_ITEM_SIZE = 512;
const short IOT_BIG_QUEUE_LENGTH = 10;
const short IOT_BIG_QUEUE_ITEM_SIZE = 2048;
    
void ConnectIoTHub();
void MonitorMessages(void * parameters);

class MessageSenderIoTHubClass
{
  public:
    MessageSenderIoTHubClass();
    void begin();
    void enqueueSmall( char * message , size_t size );
    void enqueueBig( char * message , size_t size );

  private:
    QueueHandle_t m_smallMessageQueue;
    QueueHandle_t m_bigMessageQueue;
};

extern MessageSenderIoTHubClass MessageSenderIoTHub;

#endif // __MESSAGE_SENDER_IOT_HUB_H__