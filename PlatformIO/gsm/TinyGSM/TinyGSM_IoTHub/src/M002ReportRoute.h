#ifndef __M002_REPORT_ROUTE_H__
#define __M002_REPORT_ROUTE_H__

#include "Message.h"
#include "ArduinoJson.h"


class M002ReportRoute :public Message
{
public:
	M002ReportRoute(int deviceId) :Message((uint8_t)2, deviceId) 
	{
		m_routesCount = 0;
	}
	
	String serialize()
	{
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();
		root["evnt"] = m_eventId;
		root["dev"] = m_deviceId;
		root["net"] = m_network;
		JsonArray& routes = root.createNestedArray("routes");
		for (int i = 0; i < m_routesCount; i++)
		{
			JsonObject& data = jsonBuffer.createObject();
			data["dtm"] = m_routes[i].DateTime;
			data["lat"] = m_routes[i].Latitude;
			data["lon"] = m_routes[i].Longitude;
			data["azi"] = m_routes[i].Azimuth;
			data["accx"] = m_routes[i].AccX;
			data["accy"] = m_routes[i].AccY;
			data["accz"] = m_routes[i].AccZ;
			data["gyrx"] = m_routes[i].GyroX;
			data["gyry"] = m_routes[i].GyroY;
			data["gyrz"] = m_routes[i].GyroZ;
			data["eng"] = m_routes[i].EngineState;
			routes.add(data);
		}

		String serializedText;
		root.printTo(serializedText);

		return serializedText;
	}
	
	void setNetwork(uint8_t network) { m_network = network; }
	void addRoute(RouteData_t data)
	{
		ESP_LOGI("[M002]", "Adicionando rota");

		if (m_routesCount == 0 )
		{
			ESP_LOGI("[M002]", "Alocando memoria");
			m_routesCount = 1;
			m_routes = (RouteData_t *)malloc(sizeof(RouteData_t) * m_routesCount);
		}
		else
		{
			ESP_LOGI("[M002]", "Realocando memoria");
			m_routesCount++;
			m_routes = (RouteData_t *)realloc(m_routes, sizeof(RouteData_t) * m_routesCount);
		}

		m_routes[m_routesCount - 1] = data;

		ESP_LOGI("[M002]", "Rota adicionada");
	}
private:
	uint8_t m_network;
	RouteData_t * m_routes;
	int m_routesCount;
};

#endif // __M002_REPORT_ROUTE_H__