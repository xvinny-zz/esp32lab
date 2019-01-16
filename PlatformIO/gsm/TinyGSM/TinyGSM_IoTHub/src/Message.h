#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <Arduino.h>

typedef struct
{
	uint64_t DateTime;
	float Latitude;
	float Longitude;
	uint16_t Azimuth;
	float AccX;
	float AccY;
	float AccZ;
	float GyroX;
	float GyroY;
	float GyroZ;
	uint8_t EngineState;
}RouteData_t;

typedef struct
{
	uint8_t Type;
	uint8_t Way;
	uint8_t SpeedLimit;
	uint8_t Direction;
	float Latitude;
	float Longitude;
	uint16_t Azimuth;
}PointOfInterestData_t;

using namespace std;

class Message
{
public:
	explicit Message(uint8_t eventId, int deviceId) :m_eventId(eventId), m_deviceId(deviceId) {}
	virtual String serialize() = 0;
protected:
	uint8_t m_eventId;
	int m_deviceId;

};

#endif // __MESSAGE_H__