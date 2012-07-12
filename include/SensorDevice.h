#pragma once

#include "cinder/Serial.h"
#include "Sensor.h"

namespace HeartRate
{

class SensorDevice : public Sensor
{
public:
	SensorDevice();

	bool init( const std::string &name, int baudRate );

	bool        hasMessage();
	char        getMessageId();
	std::string getMessage();

	static void printDevices();
	static std::vector<std::string> getDeviceNames();

protected:
	ci::Serial  mSerial;
};


} //namespace HeartRate

