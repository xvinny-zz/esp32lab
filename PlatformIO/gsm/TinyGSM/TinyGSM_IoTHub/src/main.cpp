#include <time.h>
#include "ConnectionManager.h"
#include "esp_log.h"
#include "MessageBook.h"
#include "LedHandler.h"

void testM001();
void testM002();
void testM003();

void setup() 
{
    LedController.begin();

    MQTTSettings_t settings;
    memset(settings.Broker, 0, sizeof(settings.Broker));
    strcpy(settings.Broker, "m13.cloudmqtt.com");
    settings.Port = 17690;
    memset(settings.User, 0, sizeof(settings.User));
    strcpy(settings.User, "gsrprydz");
    memset(settings.Password, 0, sizeof(settings.Password));
    strcpy(settings.Password, "FyKEyFOBka0U");
    memset(settings.DeviceId, 0, sizeof(settings.DeviceId));
    strcpy(settings.DeviceId, "GsmClientTest");

    esp_err_t result = ConnectionManager.begin(settings);
    if (result != ESP_OK)
    {
        ESP_LOGE("MAIN", "Falha ao iniciar o connection manager. Reiniciando...");
        esp_restart();
    }
}

void loop() 
{
    while(1)
    {
        delay(1000);
    }
    // testM001();
    // testM002();
    // testM003();
}

void testM001()
{
    M001ReportPosition test(1);
    test.setAccX(0.01);
    test.setAccY(0.02);
    test.setAccZ(0.03);
    test.setGyroX(1.1);
    test.setGyroX(1.2);
    test.setGyroX(1.3);
    time_t currentTime;
    time( &currentTime );
    test.setDateTime((uint64_t)currentTime);
    test.setLatitude(-25.123456);
    test.setLongitude(-49.654321);
    test.setNetwork(1);
    test.setEngineState(1);
    test.setExternalVoltage(12.45);
    test.setInternalVoltage(4.53);
    test.setHdop(2);
    test.setAltitude(958);
    test.setAzimuth(123);
    test.setSpeed(50);
    String content = test.serialize();
    ESP_LOGI("MAIN", "M001 Content: %s / Length: %d", content.c_str(), content.length() );
}

void testM002()
{
    M002ReportRoute test(1);
    test.setNetwork(1);

    RouteData_t data;
    
    data.AccX = 0.01;
    data.AccY = 0.02;
    data.AccZ = 0.03;
    data.GyroX = 1.1;
    data.GyroX = 1.2;
    data.GyroX = 1.3;
    time_t currentTime;
    time( &currentTime );
    data.DateTime = (uint64_t)currentTime;
    data.Latitude = -25.123456;
    data.Longitude = -49.654321;
    data.EngineState = 1;
    data.Azimuth = 123;
    test.addRoute(data);
    test.addRoute(data);
    test.addRoute(data);
    test.addRoute(data);
    test.addRoute(data);
    test.addRoute(data);
    test.addRoute(data);
    test.addRoute(data);
    test.addRoute(data);
    test.addRoute(data);
    

    String content = test.serialize();
    ESP_LOGI("MAIN", "M002 Content: %s / Length: %d", content.c_str(), content.length() );
}

void testM003()
{
    M003Infraction test(1);
    test.setAccX(0.01);
    test.setAccY(0.02);
    test.setAccZ(0.03);
    test.setGyroX(1.1);
    test.setGyroX(1.2);
    test.setGyroX(1.3);
    time_t currentTime;
    time( &currentTime );
    test.setDateTime((uint64_t)currentTime);
    test.setLatitude(-25.123456);
    test.setLongitude(-49.654321);
    test.setNetwork(1);
    test.setEngineState(1);
    test.setExternalVoltage(12.45);
    test.setInternalVoltage(4.53);
    test.setHdop(2);
    test.setAltitude(958);
    test.setAzimuth(123);
    test.setSpeed(50);
    PointOfInterestData_t point;
    point.Azimuth = 1;
    point.Direction = 1;
    point.Latitude = -25.123;
    point.Longitude = -49.123093;
    point.SpeedLimit = 60;
    point.Type = 1;
    point.Way = 1;
    test.setPointOfInterest(point);
    String content = test.serialize();
    ESP_LOGI("MAIN", "M003 Content: %s / Length: %d", content.c_str(), content.length() );
}