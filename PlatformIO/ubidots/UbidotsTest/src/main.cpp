#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "GPSHandler.h"

#define WIFI_SSID "qflux"
#define WIFI_PASS "33285901ctba"
#define TOKEN "BBFF-mCCT3WN8n8WG4f4hXDb3aOsO1xhlLU" // ubidots token
#define MQTT_CLIENT_NAME "Client000001"

/****************************************
 * Define Constants
 ****************************************/
#define VARIABLE_LABEL "sensor" // Assing the variable label
#define DEVICE_LABEL "esp32"    // Assig the device label

#define SENSOR 12 // Set the GPIO12 as SENSOR

const gpio_num_t PIN_GPS_TX = GPIO_NUM_16;  // TX do GPS
const gpio_num_t PIN_GPS_RX = GPIO_NUM_17;  // RX do GPS

char mqttBroker[] = "things.ubidots.com";
char payload[100];
char topic[150];
// Space to store values to send
char str_sensor[10];
GPSContext _GPSContext;

/****************************************
 * Auxiliar Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);

  p[length] = NULL;

  String message(p);
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");

    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, ""))
    {
      Serial.println("Connected");
    }
    else
    {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

/****************************************
 * Main Functions
 ****************************************/
void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  // Assign the pin as INPUT
  pinMode(SENSOR, INPUT);

  GPS.begin( PIN_GPS_RX , PIN_GPS_TX, 115200 );

  Serial.println();
  Serial.print("Wait for WiFi...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }

  _GPSContext = GPS.getData();

  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");                   // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label

  //float sensor = analogRead(SENSOR);
  float sensor = temperatureRead();

  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(sensor, 4, 2, str_sensor);

  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
  Serial.println("Publishing data to Ubidots Cloud");
  Serial.println(payload);
  client.publish(topic, payload);

  sprintf(payload, "%s", "");                   // Cleans the payload
  if (_GPSContext.Latitude > -1 && _GPSContext.Latitude < 1)
    sprintf(payload, "{\"position\":{\"value\":1, \"context\":{\"lat\": -25.4322864, \"lng\": -49.2472688}}}"); // Adds the variable label
  else
    sprintf(payload, "{\"position\":{\"value\":1, \"context\":{\"lat\": %.6f, \"lng\": %.6f}}}", _GPSContext.Latitude, _GPSContext.Longitude); // Adds the variable label

  Serial.println(payload);
  client.publish(topic, payload);
  client.loop();
  delay(10000);
}
