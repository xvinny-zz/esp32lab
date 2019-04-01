#include "Engine.h"

#define PIN_BUTTON_1 GPIO_NUM_39
#define PIN_BUTTON_2 GPIO_NUM_34
#define PIN_BUTTON_3 GPIO_NUM_35

QueueHandle_t g_buttonOperationQueue;
int g_buttonOperation = 0;

EngineClass::EngineClass()
{
}

void EngineClass::begin()
{
    ButtonSettings_t settings = {
        .Button1Pin = PIN_BUTTON_1,
        .Button2Pin = PIN_BUTTON_2,
        .Button3Pin = PIN_BUTTON_3
    };

    // inicializar botoes
    esp_err_t result = ButtonManager.begin(settings);
    if (result != ESP_OK)
    {
        ESP_LOGE("MAIN", "Falha ao inicializar o gerenciador de botoes.");
        while(1);
    }

    ButtonManager.attach(this);
}

void EngineClass::handle()
{
    processButtonEvents();
}

void EngineClass::OnButtonPressed(ButtonEventData_t eventData)
{
    ButtonOperation_t operation;

    switch (eventData.Source)
    {
    case BUTTON_1:
    {
        operation = OP_001;
        xQueueSend(g_buttonOperationQueue, &operation, portMAX_DELAY);
        
        ESP_LOGD(TAG_ENGINE, "Foi pressionado o botao 1 por %dms.", eventData.PressedTime);
        break;
    }
    case BUTTON_2:
        if (eventData.PressedTime > 3) // se pressionado por mais de 3 segundos, indica SOS
        {
            operation = OP_002;
            xQueueSend(g_buttonOperationQueue, &operation, portMAX_DELAY);
        }
        else // senao, indica alteracao do cronometro
        {
            operation = OP_003;
            xQueueSend(g_buttonOperationQueue, &operation, portMAX_DELAY);
        }

        // "Foi pressionado o botao 2 por %dms."
        ESP_LOGD(TAG_ENGINE, "Foi pressionado o botao 2 por %dms.", eventData.PressedTime);
        break;
    case BUTTON_3:
        if (eventData.PressedTime > 3) // se pressionado por mais de 3 segundos, indica Emergencia
        {
            operation = OP_004;
            xQueueSend(g_buttonOperationQueue, &operation, portMAX_DELAY);
        }
        else // senao, indica alteracao do odometro
        {
            operation = OP_005;
            xQueueSend(g_buttonOperationQueue, &operation, portMAX_DELAY);
        }
        ESP_LOGD(TAG_ENGINE, "Foi pressionado o botao 3 por %dms.", eventData.PressedTime);
        break;
    default:
        ESP_LOGD(TAG_ENGINE, "Botao nao reconhecido.");
    }
}

void EngineClass::processButtonEvents()
{
    // se tem evento de botão na fila...
    if (xQueueReceive(g_buttonOperationQueue, &g_buttonOperation, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        switch ((ButtonOperation_t)g_buttonOperation)
        {
        case OP_001:
            ESP_LOGD(TAG_ENGINE, "Operacao 1");
            break;
        case OP_002:
            ESP_LOGD(TAG_ENGINE, "Operacao 2");
            break;
        case OP_003:
            ESP_LOGD(TAG_ENGINE, "Operacao 3");
            break;
        case OP_004:
            ESP_LOGD(TAG_ENGINE, "Operacao 4");
            break;
        case OP_005:
            ESP_LOGD(TAG_ENGINE, "Operacao 5");
            break;
        default:
            break;
        }
    }
}

EngineClass Engine;