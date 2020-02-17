/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
 
#ifndef __WAVE_PLAYER_H__
#define __WAVE_PLAYER_H__
#define CCCC(c1, c2, c3, c4)    ((c4 << 24) | (c3 << 16) | (c2 << 8) | c1)

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "driver/i2s.h"

/* these are data structures to process wav file */
typedef enum headerState_e 
{
    HEADER_RIFF, HEADER_FMT, HEADER_DATA, DATA
} headerState_t;

typedef struct wavRiff_s {
    uint32_t chunkID;
    uint32_t chunkSize;
    uint32_t format;
} wavRiff_t;

typedef struct wavProperties_s {
    uint32_t chunkID;
    uint32_t chunkSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
} wavProperties_t;

bool WavePlayer_Initialize();
int WavePlayer_ReadRiff(File file, wavRiff_t *wavRiff);
int WavePlayer_ReadProps(File file, wavProperties_t *wavProps);
int WavePlayer_Read4bytes(File file, uint32_t *chunkId);
int WavePlayer_I2S_WriteSampleNB(uint32_t sample);
int WavePlayer_PlayFile( const char *filename );

#endif //__WAVE_PLAYER_H__
