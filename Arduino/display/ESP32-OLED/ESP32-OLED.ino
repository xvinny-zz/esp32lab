#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>

unsigned char brightness = 255;
int countX = 0;

void setup()
{
  Wire.begin();	
  Serial.begin(115200);
  oled.init();                      
  oled.clearDisplay();             
  oled.setBrightness((unsigned char)brightness);
  oled.setTextXY(3,4);
  oled.putString("########");
  oled.setTextXY(4,6);
  oled.putString("READY");
  oled.setTextXY(5,4);
  oled.putString("########");
}

void loop()
{   
  if (Serial.available()) 
  { 
    if (countX > 6)
    {
      oled.clearDisplay();
      countX = 0;
    }
    else if (countX == 0)
    {
      oled.clearDisplay();
    }
    oled.setTextXY(countX++,0);  
    oled.putString(Serial.readString());
  }
}

