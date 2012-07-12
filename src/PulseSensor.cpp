
#include <string>
#include "cinder/app/AppBasic.h"

#include "PulseSensor.h"
#include "SensorDevice.h"
#include "SensorFile.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace HeartRate
{

PulseSensor::PulseSensor()
: mpSensor( 0 )
, mpMonitor( 0 )
, mRecorder()
, mBeatPerMinute( 0 )
, mSensorData( 0 )
, mBeatPauseTime( 0 )
, mCardioid()
, mSensorName()
{
}

void PulseSensor::setup( const std::string &sensorName )
{
	mSensorName = sensorName;

	_initParam();
	getCardioid()->setup();
	getCardioid()->setVisible( true );
}

void PulseSensor::_initParam()
{
	mOutputFileName = "pulsesensor-capture-out" + mSensorName + ".xml";
	mRecording      = false;

	mDeviceNames    = SensorDevice::getDeviceNames();
	if( mpSensor )
		mDeviceName = mpSensor->getName();

	mpMonitor = new Monitor( mDeviceName, Vec2i( 350, 20 ));
	mpMonitor->AddSection( "Pulse sensor data"                   );
	mpMonitor->AddSection( "BeatPerMinute"    , &mBeatPerMinute  );
	mpMonitor->AddSection( "SensorData"       , &mSensorData     );
	mpMonitor->AddSection( "BeatPauseTimey"   , &mBeatPauseTime  );
	mpMonitor->AddSection( "Cardioid"                            );
	mpMonitor->AddSection( "Data"             , getCardioid()->getFbo());
	addCallback<Cardioid>( MT_SensorData, &Cardioid::addData, &mCardioid );

	mParams = params::PInterfaceGl( mSensorName, Vec2i( 300, 220 ) );
	mParams.addPersistentSizeAndPosition();

	// debug
	mParams.addText( "Recorder" );
	mParams.addParam( "OutputFileName", &mOutputFileName, "", true  );
	mParams.addParam( "Recording"     , &mRecording     , "", false );
	mParams.addText( "Available Devices" );
	for( vector<string>::iterator p = mDeviceNames.begin(); p != mDeviceNames.end(); ++p )
	{
		mParams.addParam( p->c_str(), &mDummy, "", true );
	}
	mParams.addText( "Actual Device" );
	mParams.addParam( "Device"     , &mDeviceName, "", true );
	mParams.addText( "Cardioid" );
	mParams.addPersistentParam( "Visible", &mVisible, getCardioid()->getVisible(), ""                          , false );
	mParams.addPersistentParam( "FactorX", &mFactorX, getCardioid()->getFactorX(), "min=0.01 max=1.0 step=0.05", false );
	mParams.addPersistentParam( "FactorY", &mFactorY, getCardioid()->getFactorY(), "min=0.01 max=1.0 step=0.05", false );
	mParams.addPersistentParam( "Offset" , &mOffset , getCardioid()->getOffset() , "min=0    max=900 step=10  ", false );
}

void PulseSensor::_updateParam()
{
	if( isRecording() != mRecording )
	{
		if( mRecording )
			startRecording( mOutputFileName );
		else
			stopRecording();
	}

	if( getCardioid()->getFactorX() != mFactorX )
		getCardioid()->setFactorX( mFactorX );

	if( getCardioid()->getFactorY() != mFactorY )
		getCardioid()->setFactorY( mFactorY );

	if( getCardioid()->getOffset() != mOffset )
		getCardioid()->setOffset( mOffset );

	if( getCardioid()->getVisible() != mVisible )
		getCardioid()->setVisible( mVisible );
}

bool PulseSensor::init( const string &deviceName, int baudRate )
{
	SensorDevice *pSensorDevice = new SensorDevice();

	if( pSensorDevice->init( deviceName, baudRate ))
	{
		if( mpSensor )
			delete mpSensor;

		mpSensor = pSensorDevice;
		return true;
	}
	else
	{
		delete pSensorDevice;
	}

	return false;
}

bool PulseSensor::init( const string &fileName )
{
	SensorFile *pSensorFile = new SensorFile();

	if( pSensorFile->init( fileName ))
	{
		if( mpSensor )
			delete mpSensor;

		mpSensor = pSensorFile;
		return true;
	}
	else
	{
		delete pSensorFile;
	}

	return false;
}

void PulseSensor::draw()
{
	if( ! mpSensor || ! mpSensor->isInited())
		return;

	if( mpMonitor )
		mpMonitor->draw();
}

void PulseSensor::update()
{
	if( ! mpSensor || ! mpSensor->isInited())
		return;

	if( mpSensor->hasMessage())
	{
		try
		{
			char   messageId = mpSensor->getMessageId();
			string message   = mpSensor->getMessage();

			_processMessage( messageId, message );
		}
		catch( ... )
		{
			console() << "Timeout while reading input from serial device!" << endl;
		}
	}

	_updateParam();
}

void PulseSensor::resize( int width, int height )
{
	if( ! mpSensor || ! mpSensor->isInited())
		return;
}

int PulseSensor::getBeatPerMinute()
{
	return mBeatPerMinute;
}

int PulseSensor::getSensorData()
{
	return mSensorData;
}

int PulseSensor::getBeatPauseTime()
{
	return mBeatPauseTime;
}

void PulseSensor::startRecording( const std::string &fileName )
{
	if( mpSensor == 0 )
		return;

	if( isRecording())
		return;

	mRecorder.startRecording( fileName, mpSensor->getName());
}

void PulseSensor::stopRecording()
{
	mRecorder.stopRecording();
}

bool PulseSensor::isRecording()
{
	return mRecorder.isRecording();
}

Cardioid *PulseSensor::getCardioid()
{
	return &mCardioid;
}

void PulseSensor::_processMessage( const char messageId, const std::string message )
{
	MessageType messageType  = _convertCharToMessageType( messageId );
	int         messageValue = atoi( message.c_str());

	switch( messageType )
	{
	case MT_BeatPerMinute : mBeatPerMinute = messageValue; break;
	case MT_SensorData    : mSensorData    = messageValue; break;
	case MT_BeatPauseTime : mBeatPauseTime = messageValue; break;
	default               : console() << "unknown message: " << message << endl; break;
	}

	mListener.callCallback( (int)messageType, messageValue );

	if( mRecorder.isRecording())
		mRecorder.dataRecording( getElapsedSeconds(), messageId, message );
}

PulseSensor::MessageType PulseSensor::_convertCharToMessageType( const char messageId )
{
	MessageType messageType = MT_Unknow;

	switch( messageId )
	{
	case 'Q': messageType = MT_BeatPerMinute; break; // 'Q' - current heart rate (beat per minute)
	case 'S': messageType = MT_SensorData;    break; // 'S' - raw sensor data
	case 'B': messageType = MT_BeatPauseTime; break; // 'B' - time between beats in milliseconds (ms time between beats)
	case '?': messageType = MT_Unknow;        break; // '?' - Unknown message type
	default : messageType = MT_Unknow;        break; // '?' - Unknown message type
	}

	return messageType;
}

char PulseSensor::_convertMessageTypeToChar( const MessageType messageType )
{
	char messageId = '?';

	switch( messageType )
	{
	case MT_BeatPerMinute : messageId = 'Q'; break; // 'Q' - current heart rate (beat per minute)
	case MT_SensorData    : messageId = 'S'; break; // 'S' - raw sensor data
	case MT_BeatPauseTime : messageId = 'B'; break; // 'B' - time between beats in milliseconds (ms time between beats)
	case MT_Unknow        : messageId = '?'; break; // '?' - Unknown message type
	default               : messageId = '?'; break; // '?' - Unknown message type
	}

	return messageId;
}

} // namespace HeartRate
