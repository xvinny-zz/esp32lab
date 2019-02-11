#include "E32TTL100.h"

HardwareSerial *g_monitorSerial;

E32TTL100Class::E32TTL100Class()
{   
}

RET_STATUS E32TTL100Class::begin(E32_PIN_t pins, HardwareSerial *monitorSerial, MODE_TYPE initialMode)
{
    RET_STATUS status = RET_SUCCESS;
    struct E32_CONFIG_t config;
    struct E32_MODULE_VERSION_t moduleVersion;

    m_pins = pins;
    
    pinMode(m_pins.M0, OUTPUT);
    pinMode(m_pins.M1, OUTPUT);
    pinMode(m_pins.Aux, INPUT);

    g_monitorSerial = monitorSerial;
    //g_monitorSerial->begin(SERIAL_BAUD_RATE);

    status = executeCommandInSleepMode(READ_CONFIG, (void *)&config);
    if (status != RET_SUCCESS)
        return status;
    
    ESP_LOGI(TAG_E32, "[%lu]Configuracoes: (Header: %02X, AddressH: %02X, AddressL: %02X, Ch: %02X)", millis(), config.Header, config.AddressHigh, config.AddressLow, config.Channel);
    ESP_LOGI(TAG_E32, "[%lu]SPED: (air_bps: %02x, uart_bps: %02X, uart_fmt: %02x)", millis(), config.SPEDConfig.air_bps, config.SPEDConfig.uart_bps, config.SPEDConfig.uart_fmt);
    ESP_LOGI(TAG_E32, "[%lu]OPTIONS: (trsm_mode: %02x, drive_mode: %02X, wakeup_time: %02x, enFWC: %02X, tsmt_pwr: %02X)", millis(), config.OptionConfig.trsm_mode, config.OptionConfig.drive_mode, config.OptionConfig.wakeup_time, config.OptionConfig.enFWC, config.OptionConfig.tsmt_pwr);

    status = configureModule(&config);
    ESP_LOGI(TAG_E32, "[%lu]Configuracao do modulo [%s]", millis(), getErrorMessage(status).c_str());
    if (status != RET_SUCCESS)
        return status;

    status = executeCommandInSleepMode(READ_MODULE_VERSION, (void *)&moduleVersion);
    if (status != RET_SUCCESS)
        ESP_LOGE(TAG_E32, "[%lu]Falha ao ler a versao do modulo. Mensagem: %s", millis(), getErrorMessage(status).c_str());
    else
        ESP_LOGI(TAG_E32, "[%lu]Modelo [%02X] | Versao: [%02X]", millis(), moduleVersion.Model, moduleVersion.Version);

    // Mode 0 | normal operation
    status = switchMode(initialMode);
    if (status != RET_SUCCESS)
    {
        ESP_LOGE(TAG_E32, "[%lu]Falha ao trocar para o modo normal. Mensagem: %s", millis(), getErrorMessage(status).c_str());
        return status;
    }
    else
        ESP_LOGI(TAG_E32, "[%lu]Modo trocado para [%s]", millis(), getModeString(initialMode).c_str());

    //self-check initialization.
    status = waitAuxPinHigh();
    if (status == RET_SUCCESS)
        ESP_LOGI(TAG_E32, "[%lu] Pino aux alto. Equipamento liberado!", millis());
    else
        ESP_LOGE(TAG_E32, "[%lu] Falha na leitura do pino Aux. Mensagem: %s", millis(), getErrorMessage(status).c_str());
    delay(10);

    return status;
}

bool E32TTL100Class::isSameMode(MODE_TYPE mode)
{
    static MODE_TYPE pre_mode = MODE_INIT;

    if (pre_mode != mode)
    {
        pre_mode = mode;
        return false;
    }

    return true;
}

RET_STATUS E32TTL100Class::readConfiguration(struct E32_CONFIG_t *pConfig)
{
    RET_STATUS STATUS = RET_SUCCESS;

    //1. read UART buffer.
    cleanUARTBuffer();

    //2. send CMD
    sendTripleCommand(READ_CONFIG);

    //3. Receive configure
    STATUS = getModuleInfo((uint8_t *)pConfig, sizeof(E32_CONFIG_t));
    if (STATUS == RET_SUCCESS)
    {
        ESP_LOGI(TAG_E32, "Settings: (Header: 0x%02x | AddH: %02X | AddL: %02X | CH: %02X)", pConfig->Header, pConfig->AddressHigh, pConfig->AddressLow, pConfig->Channel);
    }

    return STATUS;
}

void E32TTL100Class::cleanUARTBuffer()
{
    while (g_monitorSerial->available())
    {
        g_monitorSerial->read();
    }
}

void E32TTL100Class::sendTripleCommand(SLEEP_MODE_CMD_TYPE Tcmd)
{
    uint8_t CMD[3] = {Tcmd, Tcmd, Tcmd};
    g_monitorSerial->write(CMD, 3);
    delay(50); //need ti check
}

RET_STATUS E32TTL100Class::getModuleInfo(uint8_t *pReadbuf, uint8_t buf_len)
{
    ESP_LOGD(TAG_E32, "[%lu]Recebendo as informacoes do modulo...", millis());

    RET_STATUS STATUS = RET_SUCCESS;
    uint8_t availableBytes, idx;

    availableBytes = g_monitorSerial->available();
    ESP_LOGD(TAG_E32, "[%lu]Informacao do modulo com %d bytes", millis(), availableBytes);

    String receivedBytes = "";

    if (availableBytes == buf_len)
    {
        for (idx = 0; idx < buf_len; idx++)
        {
            *(pReadbuf + idx) = g_monitorSerial->read();
            receivedBytes = receivedBytes + "0x" + String(0xFF & *(pReadbuf + idx), HEX) + " ";
            //Serial.print(" 0x");
            //Serial.print(0xFF & *(pReadbuf + idx), HEX); // print as an ASCII-encoded hexadecimal
        }
        ESP_LOGD(TAG_E32, "[%lu]Bytes recebidos: %s", millis(), receivedBytes.c_str());
        //Serial.println("");
    }
    else
    {
        STATUS = RET_DATA_SIZE_NOT_MATCH;
        //Serial.print("  RET_DATA_SIZE_NOT_MATCH - Readcnt: ");
        //Serial.println(availableBytes);
        ESP_LOGE(TAG_E32, "[%lu]RET_DATA_SIZE_NOT_MATCH - Readcnt: %d", millis(), availableBytes);
        cleanUARTBuffer();
    }

    return STATUS;
}

RET_STATUS E32TTL100Class::receiveMessage(uint8_t *pDataBuffer, uint8_t *pdataLength)
{
    RET_STATUS STATUS = RET_SUCCESS;
    uint8_t idx;

    switchMode(MODE_0_NORMAL);
    *pdataLength = g_monitorSerial->available();

    if (*pdataLength > 0)
    {
        Serial.print("ReceiveMsg: ");
        Serial.print(*pdataLength);
        Serial.println(" bytes.");

        for (idx = 0; idx < *pdataLength; idx++)
            *(pDataBuffer + idx) = g_monitorSerial->read();

        for (idx = 0; idx < *pdataLength; idx++)
        {
            Serial.print(" 0x");
            Serial.print(0xFF & *(pDataBuffer + idx), HEX); // print as an ASCII-encoded hexadecimal
        }
        Serial.println("");
    }
    else
    {
        STATUS = RET_NOT_IMPLEMENT;
    }

    return STATUS;
}

RET_STATUS E32TTL100Class::sendMessageBroadCast()
{
    RET_STATUS STATUS = RET_SUCCESS;

    ESP_LOGI(TAG_E32, "[%lu]Enviando mensagem...", millis());

    switchMode(MODE_0_NORMAL);

    if (isAuxPinHigh() != HIGH)
        return RET_NOT_IMPLEMENT;

    delay(10);

    if (isAuxPinHigh() != HIGH)
        return RET_NOT_IMPLEMENT;

        //TRSM_FP_MODE
        //Send format : AddressHigh AddressLow Channel DATA_0 DATA_1 DATA_2 ...
#ifdef Device_A
    //uint8_t inputBuffer[4] = {DEVICE_B_ADDR_H, DEVICE_B_ADDR_L, 0x17, (uint8_t)random(0x00, 0x80)}; //for A
    uint8_t inputBuffer[6] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35}; //for A
#else
    uint8_t inputBuffer[4] = {DEVICE_A_ADDR_H, DEVICE_A_ADDR_L, 0x17, random(0x81, 0xFF)}; //for B
#endif
    g_monitorSerial->write(inputBuffer, sizeof(inputBuffer));

    return STATUS;
}

RET_STATUS E32TTL100Class::sendMessageBroadCast(String message)
{
    RET_STATUS STATUS = RET_SUCCESS;

    ESP_LOGI(TAG_E32, "[%lu]Enviando mensagem string...", millis());

    switchMode(MODE_0_NORMAL);

    if (isAuxPinHigh() != HIGH)
        return RET_NOT_IMPLEMENT;

    delay(10);

    if (isAuxPinHigh() != HIGH)
        return RET_NOT_IMPLEMENT;

    g_monitorSerial->print(message.c_str());

    return STATUS;
}

RET_STATUS E32TTL100Class::sendMessageBroadCast(const uint8_t * message, size_t length)
{
    RET_STATUS STATUS = RET_SUCCESS;

    ESP_LOGI(TAG_E32, "[%lu]Enviando mensagem char array...", millis());

    switchMode(MODE_0_NORMAL);

    if (isAuxPinHigh() != HIGH)
        return RET_NOT_IMPLEMENT;

    delay(10);

    if (isAuxPinHigh() != HIGH)
        return RET_NOT_IMPLEMENT;

    g_monitorSerial->write(message, length);

    return STATUS;
}

String E32TTL100Class::getModeString(MODE_TYPE mode)
{
    String modeString;

    switch(mode)
    {
        case MODE_0_NORMAL:
            modeString = "Normal";
            break;
        case MODE_1_WAKE_UP:
            modeString = "Wake Up";
            break;
        case MODE_2_POWER_SAVING:
            modeString = "Power Saving";
            break;
        case MODE_3_SLEEP:
            modeString = "Sleep";
            break;
        case MODE_INIT:
            modeString = "Init";
            break;
        default:
            modeString = "Desconhecido";
            break;
    }

    return modeString;
}

RET_STATUS E32TTL100Class::switchMode(MODE_TYPE mode)
{
    if (!isSameMode(mode))
    {
        ESP_LOGI(TAG_E32, "[%lu]Alternando para o modo %s", millis(), getModeString(mode).c_str());
        waitAuxPinHigh();

        switch (mode)
        {
        case MODE_0_NORMAL: // Mode 0 | normal operation
            digitalWrite(m_pins.M0, LOW);
            digitalWrite(m_pins.M1, LOW);
            break;
        case MODE_1_WAKE_UP: // Mode 1 | wake up operation
            digitalWrite(m_pins.M0, HIGH);
            digitalWrite(m_pins.M1, LOW);
            break;
        case MODE_2_POWER_SAVING: // Mode 2 | power saving
            digitalWrite(m_pins.M0, LOW);
            digitalWrite(m_pins.M1, HIGH);
            break;
        case MODE_3_SLEEP: // Mode 3 | Setting operation
            digitalWrite(m_pins.M0, HIGH);
            digitalWrite(m_pins.M1, HIGH);
            break;
        default:
            return RET_INVALID_PARAM;
        }

        waitAuxPinHigh();
        delay(10);
    }

    return RET_SUCCESS;
}

//return default status
RET_STATUS E32TTL100Class::waitAuxPinHigh()
{
    RET_STATUS STATUS = RET_SUCCESS;

    uint8_t count = 0;

    ESP_LOGD(TAG_E32, "[%lu]Aguardando o pino AUX ficar alto...", millis());

    while ((isAuxPinHigh() == LOW) && (count++ < TIME_OUT_CNT))
    {
        //Serial.print(".");
        delay(100);
    }

    // if (count == 0)
    // {
    // }
    //else if (count >= TIME_OUT_CNT)
    if (count >= TIME_OUT_CNT)
    {
        STATUS = RET_TIMEOUT;
        ESP_LOGE(TAG_E32, "[%lu]TimeOut aguardando o pino AUX ficar alto...", millis());
    }
    // else
    // {
    //     Serial.println("");
    // }

    return STATUS;
}

bool E32TTL100Class::isAuxPinHigh()
{
    //int val = analogRead(m_pins.Aux);
    int val = digitalRead(m_pins.Aux);

    ESP_LOGD(TAG_E32, "[%lu]Pino AUX (GPIO%02d): %d", millis(), m_pins.Aux, val);

    m_auxPinLevel = val;

    // if (val < 50)
    //     m_auxPinLevel = LOW;
    // else
    //     m_auxPinLevel = HIGH;

    return m_auxPinLevel;
}

RET_STATUS E32TTL100Class::writeConfigParameters(struct E32_CONFIG_t *pConfig)
{
    g_monitorSerial->write((uint8_t *)pConfig, 6);

    waitAuxPinHigh();
    delay(1200); //need ti check

    return RET_SUCCESS;
}

RET_STATUS E32TTL100Class::readConfig(struct E32_CONFIG_t *pConfig)
{
    ESP_LOGD(TAG_E32, "[%lu]Lendo configuracoes...", millis());

    RET_STATUS STATUS = RET_SUCCESS;

    ESP_LOGD(TAG_E32, "[%lu]Limpando buffer da UART...", millis());
    //1. read UART buffer.
    cleanUARTBuffer();
    
    ESP_LOGD(TAG_E32, "[%lu]Enviando triple command...", millis());
    //2. send CMD
    sendTripleCommand(READ_CONFIG);

    ESP_LOGD(TAG_E32, "[%lu]Recebendo as informacoes do modulo...", millis());
    //3. Receive configure
    STATUS = getModuleInfo((uint8_t *)pConfig, sizeof(E32_CONFIG_t));
    // if (STATUS == RET_SUCCESS)
    // {
    //     Serial.print("  Header:     ");
    //     Serial.println(pConfig->Header, HEX);
    //     Serial.print("  AddressHigh:     ");
    //     Serial.println(pConfig->AddressHigh, HEX);
    //     Serial.print("  AddressLow:     ");
    //     Serial.println(pConfig->AddressLow, HEX);
    //     Serial.print("  Channel:     ");
    //     Serial.println(pConfig->Channel, HEX);
    // }

    return STATUS;
}

RET_STATUS E32TTL100Class::readModuleVersion(struct E32_MODULE_VERSION_t *MVer)
{
    RET_STATUS STATUS = RET_SUCCESS;

    //1. read UART buffer.
    cleanUARTBuffer();

    //2. send CMD
    sendTripleCommand(READ_MODULE_VERSION);

    //3. Receive configure
    STATUS = getModuleInfo((uint8_t *)MVer, sizeof(E32_MODULE_VERSION_t));
    if (STATUS == RET_SUCCESS)
    {
        ESP_LOGI(TAG_E32, "[%lu] Header: 0x%02X,  Model: 0x%02X, Version: 0x%02X, Features: 0x%02X",
                            millis(), MVer->Header, MVer->Model, MVer->Version, MVer->features);
        // Serial.print("  Header:     0x");
        // Serial.println(MVer->Header, HEX);
        // Serial.print("  Model:    0x");
        // Serial.println(MVer->Model, HEX);
        // Serial.print("  Version:  0x");
        // Serial.println(MVer->Version, HEX);
        // Serial.print("  features: 0x");
        // Serial.println(MVer->features, HEX);
    }

    return RET_SUCCESS;
}

void E32TTL100Class::resetModule()
{
    sendTripleCommand(WRITE_RESET_MODULE);
    waitAuxPinHigh();
    delay(1000);
}

RET_STATUS E32TTL100Class::executeCommandInSleepMode(uint8_t CMD, void *pBuff)
{
    RET_STATUS STATUS = RET_SUCCESS;

    ESP_LOGD(TAG_E32, "[%lu]Executando o comando [0x%02X] em Sleep Mode", millis(), CMD);
    waitAuxPinHigh();

    switchMode(MODE_3_SLEEP);

    switch (CMD)
    {
    case WRITE_CONFIG_POWER_DOWN_SAVE:
        STATUS = writeConfigParameters((struct E32_CONFIG_t *)pBuff);
        break;
    case READ_CONFIG:
        STATUS = readConfig((struct E32_CONFIG_t *)pBuff);
        break;
    case WRITE_CONFIG_POWER_DOWN_LOSE:

        break;
    case READ_MODULE_VERSION:
        readModuleVersion((struct E32_MODULE_VERSION_t *)pBuff);
        break;
    case WRITE_RESET_MODULE:
        resetModule();
        break;

    default:
        return RET_INVALID_PARAM;
    }

    waitAuxPinHigh();

    return STATUS;
}

RET_STATUS E32TTL100Class::configureModule(struct E32_CONFIG_t *pCFG)
{
    RET_STATUS STATUS = RET_SUCCESS;

    ESP_LOGI(TAG_E32, "[%lu] Configurando o modulo...", millis());

#ifdef Device_A
    //pCFG->AddressHigh = DEVICE_A_ADDR_H;
    pCFG->AddressHigh = 0x00;
    //pCFG->AddressLow = DEVICE_A_ADDR_L;
    pCFG->AddressLow = 0x00;
#else
    pCFG->AddressHigh = DEVICE_B_ADDR_H;
    pCFG->AddressLow = DEVICE_B_ADDR_L;
#endif

    // TODO: alterado de modo Fixed para Transparente
    //pCFG->OptionConfig.trsm_mode = TRSM_FP_MODE;
    pCFG->OptionConfig.trsm_mode = TRSM_TT_MODE;
    pCFG->OptionConfig.tsmt_pwr = TSMT_PWR_10DB;

    STATUS = executeCommandInSleepMode(WRITE_CONFIG_POWER_DOWN_SAVE, (void *)pCFG);

    ESP_LOGI(TAG_E32, "[%lu] Resetando o modulo...", millis());
    executeCommandInSleepMode(WRITE_RESET_MODULE, NULL);

    ESP_LOGI(TAG_E32, "[%lu] Lendo as configuracoes do modulo...", millis());
    STATUS = executeCommandInSleepMode(READ_CONFIG, (void *)pCFG);

    ESP_LOGI(TAG_E32, "[%lu]Configuracoes: (Header: %02X, AddressH: %02X, AddressL: %02X, Ch: %02X)", millis(), pCFG->Header, pCFG->AddressHigh, pCFG->AddressLow, pCFG->Channel);
    ESP_LOGI(TAG_E32, "[%lu]SPED: (air_bps: %02x, uart_bps: %02X, uart_fmt: %02x)", millis(), 
                        pCFG->SPEDConfig.air_bps, pCFG->SPEDConfig.uart_bps, pCFG->SPEDConfig.uart_fmt);
    ESP_LOGI(TAG_E32, "[%lu]OPTIONS: (trsm_mode: %02x, drive_mode: %02X, wakeup_time: %02x, enFWC: %02X, tsmt_pwr: %02X)", millis(), 
                pCFG->OptionConfig.trsm_mode, pCFG->OptionConfig.drive_mode, pCFG->OptionConfig.wakeup_time, pCFG->OptionConfig.enFWC, pCFG->OptionConfig.tsmt_pwr);


    return STATUS;
}

String E32TTL100Class::getErrorMessage(RET_STATUS result)
{
    String message = "";

    switch(result)
    {
        case RET_SUCCESS:
            message = "Sucesso";
            break;
        case RET_ERROR_UNKNOWN: /* something shouldn't happened */
            message = "Erro desconhecido.";
            break;
        case RET_NOT_SUPPORT:
            message = "Nao suportado.";
            break;
        case RET_NOT_IMPLEMENT:
            message = "Nao implementado.";
            break;
        case RET_NOT_INITIAL:
            message = "Nao inicializado.";
            break;
        case RET_INVALID_PARAM:
            message = "Parametro invalido.";
            break;
        case RET_DATA_SIZE_NOT_MATCH:
            message = "O tamanho do dado esta incorreto.";
            break;
        case RET_BUF_TOO_SMALL:
            message = "Buffer muito curto.";
            break;
        case RET_TIMEOUT:
            message = "Timeout.";
            break;
        case RET_HW_ERROR:
            message = "Erro de hardware.";
            break;
        default:
            message = "Status de retorno nao reconhecido.";
            break;
    }

    return message;
}

E32TTL100Class E32TTL100;