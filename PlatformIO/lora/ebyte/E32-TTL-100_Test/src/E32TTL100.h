#ifndef __E32_TTL_100_H__
#define __E32_TTL_100_H__

#include <Arduino.h>
#include "esp_log.h"

enum MODE_TYPE
{
    MODE_0_NORMAL = 0,
    MODE_1_WAKE_UP,
    MODE_2_POWER_SAVING,
    MODE_3_SLEEP,
    MODE_INIT = 0xFF
};

typedef enum
{
    RET_SUCCESS = 0,
    RET_ERROR_UNKNOWN, /* something shouldn't happened */
    RET_NOT_SUPPORT,
    RET_NOT_IMPLEMENT,
    RET_NOT_INITIAL,
    RET_INVALID_PARAM,
    RET_DATA_SIZE_NOT_MATCH,
    RET_BUF_TOO_SMALL,
    RET_TIMEOUT,
    RET_HW_ERROR,
} RET_STATUS;

//SPED+
enum SLEEP_MODE_CMD_TYPE
{
    WRITE_CONFIG_POWER_DOWN_SAVE = 0xC0,
    READ_CONFIG = 0xC1,
    WRITE_CONFIG_POWER_DOWN_LOSE = 0xC2,
    READ_MODULE_VERSION = 0xC3,
    WRITE_RESET_MODULE = 0xC4
};

enum UART_FORMAT_TYPE
{
    UART_FORMAT_8N1 = 0x00, /*no   parity bit one stop*/
    UART_FORMAT_8O1 = 0x01, /*odd  parity bit one stop*/
    UART_FORMAT_8E1 = 0x02  /*even parity bitone stop*/
};

enum UART_BPS_TYPE
{
    UART_BPS_1200 = 0x00,
    UART_BPS_9600 = 0x03,
    UART_BPS_115200 = 0x07
};

enum AIR_BPS_TYPE
{
    AIR_BPS_300 = 0x00,
    AIR_BPS_2400 = 0x02,
    AIR_BPS_19200 = 0x05
};
//SPED-

//410~441MHz : 410M + CHAN*1M
enum AIR_CHAN_TYPE
{
    AIR_CHAN_410M = 0x00,
    AIR_CHAN_433M = 0x17,
    AIR_CHAN_441M = 0x1F
};

#define TAG_E32 "[E32-TTL-100]"

//OPTION+
#define TRSM_TT_MODE 0x00 /*Transparent Transmission*/
#define TRSM_FP_MODE 0x01 /*Fixed-point transmission mode*/

#define OD_DRIVE_MODE 0x00
#define PP_DRIVE_MODE 0x01

enum WEAK_UP_TIME_TYPE
{
    WEAK_UP_TIME_250 = 0x00,
    WEAK_UP_TIME_1000 = 0x03,
    WEAK_UP_TIME_2000 = 0x07
};

#define DISABLE_FEC 0x00
#define ENABLE_FEC 0x01

//Transmit power
enum TRANSMIT_POWER_TYPE_t
{
    TSMT_PWR_20DB = 0x00,
    TSMT_PWR_17DB = 0x01,
    TSMT_PWR_14DB = 0x02,
    TSMT_PWR_10DB = 0x03
};
//OPTION-

#pragma pack(push, 1)

struct E32_SPED_CONFIG_t
{
    uint8_t air_bps : 3;  //bit 0-2
    uint8_t uart_bps : 3; //bit 3-5
    uint8_t uart_fmt : 2; //bit 6-7
};

struct E32_OPTION_CONFIG_t
{
    uint8_t tsmt_pwr : 2;    //bit 0-1
    uint8_t enFWC : 1;       //bit 2
    uint8_t wakeup_time : 3; //bit 3-5
    uint8_t drive_mode : 1;  //bit 6
    uint8_t trsm_mode : 1;   //bit 7
};

struct E32_CONFIG_t
{
    uint8_t Header;
    uint8_t AddressHigh;
    uint8_t AddressLow;
    struct E32_SPED_CONFIG_t SPEDConfig;
    uint8_t Channel;
    struct E32_OPTION_CONFIG_t OptionConfig;
};

struct E32_MODULE_VERSION_t
{
    uint8_t Header;
    uint8_t Model;
    uint8_t Version;
    uint8_t features;
};

struct E32_PIN_t
{
    gpio_num_t Aux;
    gpio_num_t M0;
    gpio_num_t M1;
};

#pragma pack(pop)

#define Device_A

#define TIME_OUT_CNT 100
#define MAX_TX_SIZE 58

#define DEVICE_A_ADDR_H 0x05
#define DEVICE_A_ADDR_L 0x01
#define DEVICE_B_ADDR_H 0x05
#define DEVICE_B_ADDR_L 0x02

//#define M0_PIN	7
//#define M1_PIN	8
#define SERIAL_BAUD_RATE 9600ul


class E32TTL100Class
{
    public:
        E32TTL100Class();
        RET_STATUS begin(E32_PIN_t pins, HardwareSerial *monitorSerial, MODE_TYPE initialMode);
        bool isSameMode(MODE_TYPE mode);
        RET_STATUS readConfiguration(struct E32_CONFIG_t *pCFG);
        RET_STATUS getModuleInfo(uint8_t *pReadbuf, uint8_t buf_len);
        RET_STATUS receiveMessage(uint8_t *pDataBuffer, uint8_t *pdataLength);
        RET_STATUS switchMode(MODE_TYPE mode);
        RET_STATUS writeConfigParameters(struct E32_CONFIG_t* pCFG);
        RET_STATUS readConfig(struct E32_CONFIG_t* pCFG);
        RET_STATUS readModuleVersion(struct E32_MODULE_VERSION_t* MVer);
        void resetModule();
        RET_STATUS executeCommandInSleepMode(uint8_t CMD, void* pBuff);
        RET_STATUS configureModule(struct E32_CONFIG_t *pCFG);
        RET_STATUS sendMessage();
        String getErrorMessage(RET_STATUS result);
    private:
        void cleanUARTBuffer();
        void sendTripleCommand(SLEEP_MODE_CMD_TYPE Tcmd);
        RET_STATUS waitAuxPinHigh();
        bool isAuxPinHigh();
        String getModeString(MODE_TYPE mode);
        bool m_auxPinLevel;
        E32_PIN_t m_pins;
};

extern E32TTL100Class E32TTL100;

#endif