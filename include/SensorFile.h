#pragma once

#include "cinder/Xml.h"
#include "Sensor.h"

namespace HeartRate
{

class SensorFile : public Sensor
{
public:
	SensorFile();

	bool init( const std::string &fileName );

	bool        hasMessage();
	char        getMessageId();
	std::string getMessage();

protected:
	std::list<ci::XmlTree>           mListXmlData;
	std::list<ci::XmlTree>::iterator mXmlDataActIt;
	double                           mTime;
};

} //namespace HeartRate
