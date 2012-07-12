#pragma once

#include <string>
#include <vector>

namespace HeartRate
{

class Sensor
{
public:
	Sensor();
	virtual ~Sensor();

	const bool          isInited();
	const std::string   getName();

	virtual bool        hasMessage()   = 0;
	virtual char        getMessageId() = 0;
	virtual std::string getMessage()   = 0;

protected:
	bool        mInited;
	std::string mName;
};

} //namespace HeartRate
