
#include "cinder/app/AppBasic.h"
#include "SensorDevice.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace HeartRate
{

SensorDevice::SensorDevice()
: Sensor()
, mSerial()
{
}

bool SensorDevice::init( const string &deviceName, int baudRate )
{
	if( mInited )
		return false;

	try
	{
		// Serial::Device dev = Serial::findDeviceByNameContains("COM9");
		// mSerial = Serial( dev, 115200 );
		// mSerial.flush();

		Serial::Device dev = Serial::findDeviceByNameContains( deviceName );
		mSerial = Serial( dev, baudRate );
		mSerial.flush();
		mName = mSerial.getDevice().getName();
		mInited = true;
		return true;
	}
	catch( ... )
	{
		console() << "There was an error initializing the serial device: " << deviceName << endl;
	}

	return false;
}

bool SensorDevice::hasMessage()
{
	if( ! mInited )
		return false;

	if( mSerial.getNumBytesAvailable())
		return true;

	return false;
}

char SensorDevice::getMessageId()
{
	if( ! mInited )
		return '\0';

	char messageId = (char) mSerial.readByte();
	return messageId;
}

std::string SensorDevice::getMessage()
{
	if( ! mInited )
		return string();

	string message = mSerial.readStringUntil( '\n', 0, 1 );
	return message;
}

void SensorDevice::printDevices()
{
	const vector<Serial::Device> &devices( Serial::getDevices());

	for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt )
	{
		console() << "Device: " << deviceIt->getName() << endl;
	}
}

vector<string> SensorDevice::getDeviceNames()
{
	vector<string> deviceNames;
	const vector<Serial::Device> &devices( Serial::getDevices());
	for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt )
	{
		deviceNames.push_back( deviceIt->getName());
	}

	return deviceNames;
}

} // namespace HeartRate
