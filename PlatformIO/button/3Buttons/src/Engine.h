#include <Arduino.h>
#include "ButtonManager.h"
#include "EnumLibrary.h"

#define TAG_ENGINE "[ENGINE]"

#define ENGINE_LOGD_0012 "Processando botao de ultrapassagem..."
#define ENGINE_LOGD_0013 "Processando botao de cronometro..."
#define ENGINE_LOGD_0014 "Processando botao de odometro..."
#define ENGINE_LOGD_0015 "Processando botao de SOS..."
#define ENGINE_LOGD_0016 "Processando botao de emergencia..."

class EngineClass: public IButtonObserver
{
    public:
        EngineClass();
        void begin();
        void OnButtonPressed(ButtonEventData_t eventData);
        void handle();
    private:
        void processButtonEvents();
};

extern EngineClass Engine;