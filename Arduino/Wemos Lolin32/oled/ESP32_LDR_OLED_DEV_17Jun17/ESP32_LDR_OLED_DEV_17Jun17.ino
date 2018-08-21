/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 by Daniel Eichhorn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *This sketch created by JohnnyFRX 17Jun17
 */


#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"'

int sensorPin = 36;   // select the input pin for ldr
int sensorValue = 0;  // variable to store the value coming from the sensor

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 5, 4);

void setup() {
  pinMode(sensorPin, INPUT);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  delay(1000);
  display.clear();
  Serial.print(sensorValue);

  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(sensorValue));
  display.setFont(ArialMT_Plain_24);

  display.display();

  delay(10);
}

