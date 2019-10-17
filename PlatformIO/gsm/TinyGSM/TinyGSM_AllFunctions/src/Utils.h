#pragma once

#include <Arduino.h>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <iomanip>

class UtilsClass
{
  public:
    UtilsClass();
    double getArch(double turnRadius);
    esp_err_t setUnixTime(time_t value);
    time_t getUnixTime();
    esp_err_t setTime(struct tm value);
    esp_err_t getTime(struct tm *value);
    int toHexString(uint8_t *byte_array, int byte_array_len, char *hexstr, int hexstr_len);
    uint8_t* fromHexString(const char* hexstr);
  private:
    struct tm m_currentTime;
};

extern UtilsClass Utils;