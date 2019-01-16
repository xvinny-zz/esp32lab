#include <freertos/FreeRTOS.h>
#include <stdio.h>
#include <esp_log.h>
#include <driver/uart.h>

#define EX_UART_NUM     UART_NUM_2
#define PIN_GPS_TX      GPIO_NUM_16
#define PIN_GPS_RX      GPIO_NUM_17
#define BUF_SIZE        (1024)
#define RD_BUF_SIZE     (BUF_SIZE)

#define NOTASK 1

static const char *TAG = "UARTInterrupt";
static intr_handle_t handle_console;

// Receive buffer to collect incoming data
uint8_t rxbuffer[256];
static QueueHandle_t uart_queue;

static void IRAM_ATTR uart_intr_handle( void *args )
{
    uint16_t numberOfBytesToRead, status;
    uint16_t i = 0;
    BaseType_t xHigherPrioritTaskWoken = pdFALSE;
    uint8_t incomingByte = 0x00;

    status = UART2.int_st.val;  // UART interrupt status
    numberOfBytesToRead = UART2.status.rxfifo_cnt; // number of bytes in UART buffer

    while ( numberOfBytesToRead )
    {
        //rxbuffer[i++] = UART2.fifo.rw_byte;
        incomingByte = UART2.fifo.rw_byte;
        //xQueueSendFromISR (uart_queue, &rxbuffer[i], xHigherPrioritTaskWoken );
        xQueueSendFromISR (uart_queue, &incomingByte, xHigherPrioritTaskWoken );
        numberOfBytesToRead--;
    }

    // depois de ler os bytes da fila, limpa o status de interrupcao
    uart_clear_intr_status( EX_UART_NUM, UART_RXFIFO_FULL_INT_CLR | UART_RXFIFO_TOUT_INT_CLR );

    //ESP_LOGI( TAG, "RX done. Status: %X | Bytes read: %d", status, i );
}

static void uart_event_task(void *pvParameters)
{
    uint8_t element = 0x00;

    while(true)
    {
        if( xQueueReceive(uart_queue, &element, (portTickType)portMAX_DELAY) == pdTRUE ) 
        {
            printf( "%c", element );
            if (element == 0x0a)
                printf( "\n" );
        }
    }

    vTaskDelete( NULL );
}

void app_main()
{
    esp_err_t result = ESP_OK;

    uart_queue = xQueueCreate( 10240, sizeof(uint8_t) );

    // Definir o nivel de log para INFO
    esp_log_level_set( TAG, ESP_LOG_INFO );
    ESP_LOGI( TAG, "Setting up log level to ESP_LOG_INFO" );

    // Criar as configuracoes da porta serial
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    // Aplicar as configuracoes da porta serial
    ESP_LOGI( TAG, "Setting up serial config..." );
    result = uart_param_config( EX_UART_NUM, &uart_config );
    if (result != ESP_OK)
    {
        ESP_LOGE( TAG , "Erro configurando a porta serial. Codigo: %X", result );
        esp_restart();
    }

    // Configurar os pinos da porta serial
    ESP_LOGI( TAG, "Setting up serial pins..." );
    result = uart_set_pin( EX_UART_NUM , PIN_GPS_TX, PIN_GPS_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE );
    if (result != ESP_OK)
    {
        ESP_LOGE( TAG , "Erro definindo os pinos da porta serial. Codigo: %X", result  );
        esp_restart();
    }

    // Instalar o driver da serial
    ESP_LOGI( TAG, "Installing serial driver..." );
    result = uart_driver_install( EX_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0 );
    if (result != ESP_OK)
    {
        ESP_LOGE( TAG , "Erro instalando o driver da porta serial. Codigo: %X", result  );
        esp_restart();
    }

    // Liberar a interrupcao da serial antes de registra-la
    ESP_LOGI( TAG, "Releasing serial interrupt..." );
    result = uart_isr_free(EX_UART_NUM);
    if (result != ESP_OK)
    {
        ESP_LOGE( TAG , "Erro liberando a interrupcao da porta serial. Codigo: %X", result  );
        esp_restart();
    }

    // Registrar a interrupcao da serial
    ESP_LOGI( TAG, "Registering serial interrupt..." );
    result = uart_isr_register(EX_UART_NUM, uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console);
    if (result != ESP_OK)
    {
        ESP_LOGE( TAG , "Erro registrando a interrupcao da porta serial. Codigo: %X", result  );
        esp_restart();
    }

    // Habilitar a interrupcao serial
    ESP_LOGI( TAG, "Enabling serial interrupt..." );
    result = uart_enable_rx_intr( EX_UART_NUM );
    if (result != ESP_OK)
    {
        ESP_LOGE( TAG , "Erro habilitando a interrupcao da porta serial. Codigo: %X", result  );
        esp_restart();
    }

    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}