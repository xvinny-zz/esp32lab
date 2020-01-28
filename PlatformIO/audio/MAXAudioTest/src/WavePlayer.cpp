#include "WavePlayer.h"

File _wavFile;
headerState_t _headerState = HEADER_RIFF;
wavProperties_t _wavProps;
int _i2sPortNumber = 0; // i2s port number
i2s_config_t _i2sConfig = {
     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
     .sample_rate = 22050,
     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
     .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
     .dma_buf_count = 8,
     .dma_buf_len = 64   //Interrupt level 1
    };

i2s_pin_config_t i2sPinConfig = {
    .bck_io_num = 26, //this is BCK pin
    .ws_io_num = 25, // this is LRCK pin
    .data_out_num = 22, // this is DATA output pin
    .data_in_num = -1   //Not used
};

bool WavePlayer_Initialize()
{
    Serial.println( "Inicializando sistema de audio..." );
    
    Serial.println( "Inicializando o SD card..." );
    if( !SD.begin() )
    {
        Serial.println("Falha ao inicializar o SD card");
        return false;
    }

    uint8_t cardType = SD.cardType();

    if( cardType == CARD_NONE )
    {
        Serial.println( "Nenhum SD card foi encontrado." );
        return false;
    }

    Serial.print( "Tipo do SD Card: " );
    
    if( cardType == CARD_MMC )
        Serial.println( "MMC" );
    else if(cardType == CARD_SD)
        Serial.println( "SDSC" );
    else if(cardType == CARD_SDHC)
        Serial.println( "SDHC" );
    else
        Serial.println( "Desconhecido" );

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf( "Tamanho do SD card: %lluMB\n", cardSize );

    //initialize i2s with configurations above
    i2s_driver_install( ( i2s_port_t ) _i2sPortNumber, &_i2sConfig, 0, NULL );
    i2s_set_pin( ( i2s_port_t ) _i2sPortNumber, &i2sPinConfig );
    
    //set sample rates of i2s to sample rate of wav file
    //i2s_set_sample_rates( (i2s_port_t) _i2sPortNumber, _wavProps.sampleRate ); 
    i2s_set_sample_rates( ( i2s_port_t ) _i2sPortNumber, 11025 ); 

    Serial.println( "Sistema de audio inicializado!" );
    
    return true;
}

bool WavePlayer_Finalize()
{
    
}

/* these are function to process wav file */
int WavePlayer_ReadRiff( File file, wavRiff_t *wavRiff )
{
  int n = file.read( ( uint8_t * ) wavRiff, sizeof( wavRiff_t ) );
  return n;
}

int WavePlayer_ReadProps( File file, wavProperties_t *wavProps )
{
  int n = file.read( ( uint8_t * ) wavProps, sizeof( wavProperties_t ) );
  return n;
}

/* read 4 bytes of data from wav file */
int WavePlayer_Read4bytes( File file, uint32_t *chunkId )
{
  int n = file.read( ( uint8_t * ) chunkId, sizeof( uint32_t ) );
  return n;
}

/* write sample data to I2S */
int WavePlayer_I2S_WriteSampleNB( uint32_t sample )
{
    return i2s_write_bytes( ( i2s_port_t ) _i2sPortNumber, ( const char * )&sample, sizeof( uint32_t ), 100 );
}

int WavePlayer_PlayFile( const char *filename )
{
    Serial.printf( "Reproduzindo o arquivo de audio %s...\n" , filename );
    
    _wavFile = SD.open( filename );
    
    if( !_wavFile )
    {
        Serial.printf( "Erro: O arquivo %s nao foi encontrado\n", filename );
        return -1;
    }
        
    int c = 0;
    int n;
        
    while( _wavFile.available() )
    {
        switch( _headerState )
        {
            case HEADER_RIFF:
                wavRiff_t wavRiff;
                n = WavePlayer_ReadRiff( _wavFile, &wavRiff );
                
                if( n == sizeof( wavRiff_t ) )
                {
                    if(wavRiff.chunkID == CCCC( 'R', 'I', 'F', 'F' ) && wavRiff.format == CCCC( 'W', 'A', 'V', 'E' ) )
                        _headerState = HEADER_FMT;
                }
            break;
            case HEADER_FMT:
                n = WavePlayer_ReadProps( _wavFile, &_wavProps );
                
                if( n == sizeof( wavProperties_t ) )
                    _headerState = HEADER_DATA;
                
                break;
            case HEADER_DATA:
                uint32_t chunkId, chunkSize;
                
                n = WavePlayer_Read4bytes( _wavFile , &chunkId );
                
                if(n == 4)
                {
                    if(chunkId == CCCC( 'd', 'a', 't', 'a' ))
                        Serial.println( "HEADER_DATA" );
                }
                
                n = WavePlayer_Read4bytes( _wavFile , &chunkSize );
                
                if(n == 4)
                    _headerState = DATA;

                /*
                //initialize i2s with configurations above
                i2s_driver_install( ( i2s_port_t ) _i2sPortNumber, &_i2sConfig, 0, NULL );
                i2s_set_pin( ( i2s_port_t ) _i2sPortNumber, &i2sPinConfig );
                
                //set sample rates of i2s to sample rate of wav file
                //i2s_set_sample_rates( (i2s_port_t) _i2sPortNumber, _wavProps.sampleRate ); 
                i2s_set_sample_rates( ( i2s_port_t ) _i2sPortNumber, 11025 ); 
                */
                
                break; 
            /* after processing wav file, it is time to process music data */
            case DATA:
                uint32_t data;
                n = WavePlayer_Read4bytes( _wavFile , &data );
                
                /*printf( "\nReproduzindo %d bytes de audio lidos:", n );

                char * sample = (char *)&data;
                for (int i = 0 ; i < n ; i++)
                  printf("%d=%02X ",sample[i],sample[i]);*/
                
                
                WavePlayer_I2S_WriteSampleNB( data );
                
                break;
          }
    }

    _wavFile.close();

    Serial.printf( "Arquivo %s reproduzido!\n", filename );

    //i2s_driver_uninstall( ( i2s_port_t )_i2sPortNumber ); //stop & destroy i2s driver 

    return 0;
}
