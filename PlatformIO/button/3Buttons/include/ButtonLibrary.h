#ifndef __BUTTON_LIBRARY_H__
#define __BUTTON_LIBRARY_H__

#include <Arduino.h>

enum ButtonSource_t
{
    BUTTON_1,
    BUTTON_2,
    BUTTON_3
};

struct ButtonEventData_t
{
    ButtonSource_t Source;  // botao que foi pressionado
    int PressedTime;        // tempo que o botao ficou pressionado
};

struct ButtonSettings_t
{
    gpio_num_t Button1Pin;
    gpio_num_t Button2Pin;
    gpio_num_t Button3Pin;
    gpio_num_t Button1Led;
    gpio_num_t Button2Led;
    gpio_num_t Button3Led;
};

#endif // __BUTTON_LIBRARY_H__