
#include "libGSM.h"

#define GSM_APN "timbrasil.br"
#define GSM_USER "tim"
#define GSM_PASSWORD "tim"

void setup() 
{
    Serial.begin(115200);
    setPPPosConfig( GSM_APN, GSM_USER, GSM_PASSWORD );
    ppposInit();
}

void loop() 
{
    // put your main code here, to run repeatedly:
    if ( ppposStatus() != GSM_STATE_CONNECTED )
    {
        Serial.println("GSM nao conectado...");
    }
    else
    {
        Serial.println("GSM conectado!");
    }

}
