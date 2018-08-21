/*
  This is a simple example show the LoRa recived data in OLED.
  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.
  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include "SSD1306.h" 
#include "images.h"

// Pin definetion of WIFI LoRa 32
// HelTec AutoMation 2017 support@heltec.cn 
#define SCK     5    // GPIO5  -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    27   // GPIO27 -- SX127x's MOSI
#define SS      18   // GPIO18 -- SX127x's CS
#define RST     14   // GPIO14 -- SX127x's RESET
#define DI00    26   // GPIO26 -- SX127x's IRQ(Interrupt Request)

#define BAND    433E6  //you can set band here directly,e.g. 433E6, 868E6,915E6
#define PABOOST true

// Definições do OLED
#define OLED_RST      16  // GPIO16 -- OLED RESET
#define OLED_ADDRESS  0x3C  // endereço do OLED
#define OLED_SDA      4     // GPIO04 - SDA do OLED
#define OLED_SCL      15    // GPIO15 - SCL do OLED

SSD1306 display( OLED_ADDRESS, OLED_SDA, OLED_SCL );
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void logo()
{
  int16_t x = 0;
  int16_t y = 5;
  
  display.clear();
  display.drawXbm( x, y, logo_width, logo_height, (const uint8_t *)logo_bits );
  display.display();
}

void loraData()
{
  display.clear();
  display.setTextAlignment( TEXT_ALIGN_LEFT );
  display.setFont( ArialMT_Plain_10 );
  display.drawString( 0 , 15 , "Received " + packSize + " bytes" );
  display.drawStringMaxWidth( 0 , 26 , 128, packet );
  display.drawString( 0, 0, rssi );  
  display.display();
}

void cbk( int packetSize ) 
{
  packet = "";
  packSize = String( packetSize, DEC );
  for( int i = 0; i < packetSize; i++ ) 
  { 
    packet += (char) LoRa.read(); 
  }
  
  rssi = "RSSI " + String( LoRa.packetRssi(), DEC );
  loraData();
}

void setup() 
{
  pinMode( OLED_RST, OUTPUT );    // define o pino OLED_RST como output
  digitalWrite( OLED_RST, LOW );  // seta o GPIO16 pra BAIXO pra resetar o OLED
  delay( 50 );                    // aguarda 50ms
  digitalWrite( OLED_RST, HIGH ); // enquanto o OLED estiver rodando, o GPIO16 deve estar em ALTO
  display.init();                 // inicializa o display
  display.flipScreenVertically(); // gira o display verticalmente
  display.setFont(ArialMT_Plain_10);  // configura a fonte para Arial 10
  
  logo();                             // joga a logo no display, aguarda 1.5s e depois limpa a tela
  delay( 1500 );
  display.clear();
  
  SPI.begin( SCK, MISO, MOSI, SS );        // inicializa a SPI pra comunicar com a interface LoRa
  LoRa.setPins( SS, RST, DI00 );          // configura os pinos SS, RST e DI00 do LoRa
  
  if (!LoRa.begin( BAND, PABOOST ))      // Inicializa a interface LoRa
  {
    display.drawString(0, 0, "Starting LoRa failed!");
    display.display();
    while (1);
  }
  
  display.drawString(0, 0, "LoRa Initial success!");
  display.drawString(0, 10, "Wait for incomm data...");
  display.display();
  delay(1000);
  //LoRa.onReceive(cbk);
  
  LoRa.receive();   // Inicializa a interface de recebimento do LoRa
}

void loop() 
{
  int packetSize = LoRa.parsePacket();      // Faz o parse de um pacote LoRa
  if (packetSize) 
  { 
    cbk(packetSize);  
  }
  
  delay(10);
}
