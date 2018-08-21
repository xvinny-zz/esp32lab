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
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "driver/i2s.h"
#include "freertos/queue.h"

#define CCCC(c1, c2, c3, c4)    ((c4 << 24) | (c3 << 16) | (c2 << 8) | c1)

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

File _file;
headerState_t state = HEADER_RIFF;
wavProperties_t wavProps;

//i2s configuration 
int i2s_num = 0; // i2s port number
i2s_config_t i2s_config = {
     .mode = (i2s_mode_t)( I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN ),
     .sample_rate = 22050,
     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
     .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S_MSB),
     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
     .dma_buf_count = 8,
     .dma_buf_len = 64   //Interrupt level 1
    };

/* these are function to process wav file */
int readRiff(File file, wavRiff_t *wavRiff)
{
  int n = file.read((uint8_t *)wavRiff, sizeof(wavRiff_t));
  return n;
}

int readProps(File file, wavProperties_t *wavProps)
{
  int n = file.read((uint8_t *)wavProps, sizeof(wavProperties_t));
  return n;
}

/* read 4 bytes of data from wav file */
int read4bytes(File file, uint32_t *chunkId)
{
  int n = file.read((uint8_t *)chunkId, sizeof(uint32_t));
  return n;
}

/* write sample data to I2S */
int i2s_write_sample_nb(uint32_t sample)
{
  return i2s_write_bytes((i2s_port_t)i2s_num, (const char *)&sample, sizeof(uint32_t), 100);
}

void setup(){
  int count = 0;
  
    Serial.begin(115200);
    Serial.print("Initializing SD card...");
    
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    _file = SD.open("/sinfonia.wav");
    if(!_file){
        Serial.println("Failed to open file for reading");
        return;
    }
    else
    {
        Serial.print("Read from file: ");
        int c = 0;
        int n;
        
        while(_file.available())
        {
            switch(state)
            {
                case HEADER_RIFF:
                    wavRiff_t wavRiff;
                    n = readRiff( _file, &wavRiff );
                    
                    if( n == sizeof(wavRiff_t) )
                    {
                        if(wavRiff.chunkID == CCCC('R', 'I', 'F', 'F') && wavRiff.format == CCCC('W', 'A', 'V', 'E'))
                        {
                          state = HEADER_FMT;
                          Serial.println("HEADER_RIFF");
                        }
                    }
                break;
                case HEADER_FMT:
                    n = readProps( _file, &wavProps );
                    
                    if( n == sizeof( wavProperties_t ) )
                    {
                        state = HEADER_DATA;
                    }
                    break;
                case HEADER_DATA:
                    uint32_t chunkId, chunkSize;
                    
                    n = read4bytes( _file , &chunkId );
                    
                    if(n == 4)
                    {
                        if(chunkId == CCCC('d', 'a', 't', 'a'))
                        {
                            Serial.println("HEADER_DATA");
                        }
                    }
                    
                    n = read4bytes( _file , &chunkSize );
                    
                    if(n == 4)
                    {
                        Serial.println( "start playing..." );
                        state = DATA;
                    }
                    
                    //initialize i2s with configurations above
                    i2s_driver_install( (i2s_port_t) i2s_num, &i2s_config, 0, NULL );
                    i2s_set_pin( (i2s_port_t) i2s_num, NULL );
                    
                    //set sample rates of i2s to sample rate of wav file
                    Serial.print("Sample rate");
                    Serial.println(wavProps.sampleRate);
                    //i2s_set_sample_rates( (i2s_port_t) i2s_num, wavProps.sampleRate ); 
                    i2s_set_sample_rates( (i2s_port_t) i2s_num, 11025 ); 
                    
                    break; 
                /* after processing wav file, it is time to process music data */
                case DATA:
                    uint32_t data; 

                    n = read4bytes( _file , &data );
                    i2s_write_sample_nb( data ); 
                    
                    break;
              }
        }

        _file.close();
    }

    i2s_driver_uninstall((i2s_port_t)i2s_num); //stop & destroy i2s driver 

    Serial.println( "played!" );
}

void loop()
{

}
