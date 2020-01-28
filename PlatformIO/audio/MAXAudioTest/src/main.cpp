#include "WavePlayer.h"

void setup()
{
    Serial.begin( 115200 );
    
    if (WavePlayer_Initialize() == false)
        esp_restart();
}

void loop()
{
    WavePlayer_PlayFile( "/systemon.wav" ); 
    delay(1000);

    WavePlayer_PlayFile( "/slowto.wav" ); 
    WavePlayer_PlayFile( "/20.wav" ); 
    WavePlayer_PlayFile( "/kmh.wav" ); 
    WavePlayer_PlayFile( "/left.wav" ); 
    delay(1000);

    WavePlayer_PlayFile( "/slowto.wav" ); 
    WavePlayer_PlayFile( "/30.wav" ); 
    WavePlayer_PlayFile( "/kmh.wav" ); 
    WavePlayer_PlayFile( "/right.wav" ); 
    delay(1000);

    WavePlayer_PlayFile( "/slowto.wav" ); 
    WavePlayer_PlayFile( "/40.wav" ); 
    WavePlayer_PlayFile( "/kmh.wav" ); 
    WavePlayer_PlayFile( "/left.wav" ); 
    delay(1000);

    WavePlayer_PlayFile( "/slowto.wav" ); 
    WavePlayer_PlayFile( "/50.wav" ); 
    WavePlayer_PlayFile( "/kmh.wav" ); 
    WavePlayer_PlayFile( "/right.wav" ); 
    delay(1000);

    WavePlayer_PlayFile( "/slowto.wav" ); 
    WavePlayer_PlayFile( "/60.wav" ); 
    WavePlayer_PlayFile( "/kmh.wav" ); 
    WavePlayer_PlayFile( "/left.wav" ); 
    delay(1000);

    WavePlayer_PlayFile( "/slowto.wav" ); 
    WavePlayer_PlayFile( "/70.wav" ); 
    WavePlayer_PlayFile( "/kmh.wav" ); 
    WavePlayer_PlayFile( "/right.wav" ); 
    delay(1000);

    WavePlayer_PlayFile( "/slownow.wav" ); 
    delay(1000);
}
