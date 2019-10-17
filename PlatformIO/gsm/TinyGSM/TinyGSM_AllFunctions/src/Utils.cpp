#include "Utils.h"

UtilsClass::UtilsClass()
{
}

double UtilsClass::getArch(double turnRadius) 
{
	return turnRadius / 3 * 3.1416;
}



esp_err_t UtilsClass::setUnixTime(time_t value)
{
    struct timeval now = {.tv_sec = value};
    settimeofday(&now, NULL);

    return ESP_OK;
}

time_t UtilsClass::getUnixTime()
{
    time_t myTime;
    time(&myTime);
    
    return myTime;
}

esp_err_t UtilsClass::setTime(struct tm value)
{
    ESP_LOGD("UTILS", "Configurando o horario: Data/Hora: %02d/%02d/%d %02d:%02d:%02d", 
        value.tm_mday, value.tm_mon, value.tm_year, 
        value.tm_hour, value.tm_min, value.tm_sec);

    time_t t = mktime(&value);
    setUnixTime(t);

    return ESP_OK;
}

esp_err_t UtilsClass::getTime(struct tm *value)
{
    time_t rawtime;

    rawtime = getUnixTime();
    value = localtime(&rawtime);

    return ESP_OK;
}

int UtilsClass::toHexString(uint8_t *byte_array, int byte_array_len, char *hexstr, int hexstr_len)
{
    int off = 0;
    int i;

    for (i = 0; i < byte_array_len; i++) {
        off += snprintf(hexstr + off, hexstr_len - off, "%02x", byte_array[i]);
    }

    hexstr[off+1] = '\0';

    return off;
}

uint8_t* UtilsClass::fromHexString(const char* hexstr)
{
    size_t len = strlen(hexstr);
    if(len % 2 != 0)
        return NULL;

    size_t final_len = len / 2;
    uint8_t* chrs = (unsigned char*)malloc((final_len+1) * sizeof(*chrs));
    for (size_t i=0, j=0; j<final_len; i+=2, j++)
        chrs[j] = (hexstr[i] % 32 + 9) % 25 * 16 + (hexstr[i+1] % 32 + 9) % 25;

    chrs[final_len] = '\0';
    return chrs;
}

UtilsClass Utils;