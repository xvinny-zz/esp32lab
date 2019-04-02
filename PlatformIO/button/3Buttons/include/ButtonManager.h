#ifndef __BUTTON_MANAGER_H__
#define __BUTTON_MANAGER_H__

#include <Arduino.h>
#include <vector>
#include <list>
#include <algorithm>

#include "ButtonLibrary.h"
#include "MessageLibrary.h"
#include "IButtonObserver.h"

// Tempo de espera do debounce dos eventos dos botões, em milissegundos.
#define DEBOUNCE_WAIT_TIME 20

class ButtonManagerClass
{
    public:
        ButtonManagerClass();
        esp_err_t begin(ButtonSettings_t settings);
        void attach(IButtonObserver *observer);
        void detach(IButtonObserver *observer);
        void notify(ButtonEventData_t eventData);    
    private:
        std::vector<IButtonObserver*> m_list;
};

extern ButtonManagerClass ButtonManager;


#endif // __BUTTON_MANAGER_H__