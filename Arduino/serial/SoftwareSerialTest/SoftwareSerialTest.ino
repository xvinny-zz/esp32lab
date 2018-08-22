#include <SoftwareSerial.h>

SoftwareSerial swSer(22, 23, false, 256);

void setup() {
  Serial.begin(115200);
  swSer.begin(9600);

  Serial.println("\nSoftware serial test started");

}

void loop() {
  while (swSer.available() > 0) {
    Serial.write(swSer.read());
  }

}
