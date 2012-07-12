#pragma once

#include "cinder/Xml.h"

namespace HeartRate
{

class Recorder
{
public:
	Recorder();

public:
	void startRecording( const std::string &fileName, const std::string &name );
	void stopRecording();
	bool isRecording();
	void dataRecording( const double time, const char messageType, const std::string &message );
	void testRecording();

private:
	std::ofstream *mpXmlFile;
	ci::XmlTree    mXmlTree;
	ci::fs::path   mPath;
	bool           mRecording;
	double         mTime;
};

} //namespace HeartRate

