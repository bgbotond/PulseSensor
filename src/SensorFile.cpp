
#include "cinder/app/AppBasic.h"
#include "SensorFile.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace HeartRate
{

SensorFile::SensorFile()
: Sensor()
, mListXmlData()
, mXmlDataActIt()
, mPosXmlDataAct( 0 )
, mTime( 0.0 )
{
}

bool SensorFile::init( const string &fileName )
{
	if( mInited )
		return false;

	fs::path fullPath( getAssetPath( fileName ));
	if( ! fs::exists( fullPath ))
	{
		console() << "Unable to load captured file: " << fileName << endl;
		return false;
	}

	XmlTree doc( loadFile( fullPath ));

	if( doc.hasChild( "PulseSensor" ))
	{
		XmlTree xmlPulseSensor = doc.getChild( "PulseSensor" );
		mName = xmlPulseSensor.getAttributeValue<string>( "Name"    , "" );

		if( xmlPulseSensor.hasChild( "Data" ))
		{
			mListXmlData   = xmlPulseSensor.getChildren();
			mPosXmlDataAct = 0;
			mInited        = true;
			mTime          = getElapsedSeconds();
			return true;
		}
	}

	console() << "Invalid captured file: " << fileName << endl;
	return false;
}

bool SensorFile::hasMessage()
{
	if( ! mInited )
		return false;

	std::list<ci::XmlTree>::iterator xmlDataActIt = std::next( mListXmlData.begin(), mPosXmlDataAct );

	double time = xmlDataActIt->getAttributeValue<double>( "Time", 0.0 );

	if( time + mTime <= getElapsedSeconds())
	{
		mXmlDataActIt = xmlDataActIt;
		mPosXmlDataAct++;
		if( mPosXmlDataAct >= mListXmlData.size())
		{
			mPosXmlDataAct = 0;
			mTime          = getElapsedSeconds();
		}
		return true;
	}

	return false;
}

char SensorFile::getMessageId()
{
	if( ! mInited )
		return '\0';

	char messageId = mXmlDataActIt->getAttributeValue<char>( "Type", '?' );
	return messageId;
}

std::string SensorFile::getMessage()
{
	if( ! mInited )
		return string();

	string message = mXmlDataActIt->getAttributeValue<string>( "Data", "" );
	return message;
}

} // namespace HeartRate
