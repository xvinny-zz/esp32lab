/*
  This is a simple example show the LoRa sended data in OLED.

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

#define LED_INFO      25    // GPIO25 - LED informativo

unsigned int counter = 0;

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

void setup()
{
  pinMode( OLED_RST, OUTPUT );    // define o pino OLED_RST como output
  pinMode( LED_INFO, OUTPUT );
  
  digitalWrite( OLED_RST, LOW );  // seta o GPIO16 pra BAIXO pra resetar o OLED
  delay( 50 );                    // aguarda 50ms
  digitalWrite( OLED_RST, HIGH ); // enquanto o OLED estiver rodando, o GPIO16 deve estar em ALTO

  display.init();
  display.flipScreenVertically();  
  display.setFont( ArialMT_Plain_10 );
  logo();
  delay( 1500 );
  display.clear();
  
  SPI.begin( SCK, MISO, MOSI, SS );
  LoRa.setPins( SS, RST, DI00 );
  
  if ( !LoRa.begin( BAND, PABOOST ) )
  {
    display.drawString( 0, 0, "Starting LoRa failed!" );
    display.display();
    while (1);
  }
  display.drawString( 0, 0, "LoRa Initial success!" );
  display.display();
  delay(1000);
}

void loop()
{
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  
  display.drawString( 0, 0, "Sending packet: " );
  display.drawString( 90, 0, String(counter) );
  display.display();

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print( counter );
  LoRa.endPacket();

  counter++;
  digitalWrite( 25, HIGH );   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite( 25, LOW );    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
