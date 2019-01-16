#ifndef __M001_REPORT_POSITION_H__
#define __M001_REPORT_POSITION_H__

#include "Message.h"
#include "ArduinoJson.h"


class M001ReportPosition :public Message
{
public:
	M001ReportPosition(int deviceId) :Message((uint8_t)1, deviceId) {}
	String serialize()
	{
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();
		root["evnt"] = m_eventId;
		root["dev"] = m_deviceId;
		root["net"] = m_network;
		root["dtm"] = m_dateTime;
		root["lat"] = m_latitude;
		root["lon"] = m_longitude;
		root["azi"] = m_azimuth;
		root["hdop"] = m_hdop;
		root["alt"] = m_altitude;
		root["spd"] = m_speed;
		root["extv"] = m_externalVoltage;
		root["intv"] = m_internalVoltage;
		root["accx"] = m_accX;
		root["accy"] = m_accY;
		root["accz"] = m_accZ;
		root["gyrx"] = m_gyroX;
		root["gyry"] = m_gyroY;
		root["gyrz"] = m_gyroZ;
		root["eng"] = m_engineState;
		String serializedText;
		root.printTo(serializedText);
		return serializedText;
	}
	void setNetwork(uint8_t network) { m_network = network; }
	void setDateTime(uint64_t dateTime) { m_dateTime = dateTime; }
	void setLatitude(float latitude) { m_latitude = latitude; }
	void setLongitude(float longitude) { m_longitude = longitude; }
	void setAzimuth(uint16_t azimuth) { m_azimuth = azimuth; }
	void setHdop(uint8_t hdop) { m_hdop = hdop; }
	void setAltitude(uint16_t altitude) { m_altitude = altitude; }
	void setSpeed(int speed) { m_speed = speed; }
	void setExternalVoltage(float voltage) { m_externalVoltage = voltage; }
	void setInternalVoltage(float voltage) { m_internalVoltage = voltage; }
	void setAccX(float value) { m_accX = value; }
	void setAccY(float value) { m_accY = value; }
	void setAccZ(float value) { m_accZ = value; }
	void setGyroX(float value) { m_gyroX = value; }
	void setGyroY(float value) { m_gyroY = value; }
	void setGyroZ(float value) { m_gyroZ = value; }
	void setEngineState(uint8_t state) { m_engineState = state; }
private:
	uint8_t m_network;
	uint64_t m_dateTime;
	float m_latitude;
	float m_longitude;
	uint16_t m_azimuth;
	uint8_t m_hdop;
	uint16_t m_altitude;
	int m_speed;
	float m_externalVoltage;
	float m_internalVoltage;
	float m_accX;
	float m_accY;
	float m_accZ;
	float m_gyroX;
	float m_gyroY;
	float m_gyroZ;
	uint8_t m_engineState;
};

#endif // __M001_REPORT_POSITION_H__