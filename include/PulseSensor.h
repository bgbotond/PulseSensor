#pragma once

#include "cinder/params/Params.h"
#include "cinder/Xml.h"

#include "PParams.h"
#include "Sensor.h"
#include "Recorder.h"
#include "Cardioid.h"
#include "Monitor.h"
#include "Listener.h"

namespace HeartRate
{

class PulseSensor
{
public:
	enum MessageType
	{
		MT_BeatPerMinute,              // 'Q' - current heart rate (beat per minute)
		MT_SensorData,                 // 'S' - raw sensor data
		MT_BeatPauseTime,              // 'B' - time between beats in milliseconds (ms time between beats)
		MT_Unknow,                     //  ?  - unknown message type
	};

public:
	PulseSensor();

	bool init( const std::string &deviceName, int baudRate );
	bool init( const std::string &fileName                 );

	void setup( const std::string &sensorName );
	void draw();
	void update();
	void resize( int width, int height );

	int  getBeatPerMinute();
	int  getSensorData();
	int  getBeatPauseTime();

	void startRecording( const std::string &fileName );
	void stopRecording();
	bool isRecording();

	Cardioid *getCardioid();

	template<typename T>
	void addCallback( MessageType messageType, void (T::* callbackFunction)( int value ), T* callbackObject )
	{
		mListener.addCallback<T>( (int)messageType, callbackFunction, callbackObject );
	}

protected:
	void         _processMessage( const char messageId, const std::string message );

	MessageType  _convertCharToMessageType( const char        messageId   );
	char         _convertMessageTypeToChar( const MessageType messageType );

	void         _initParam();
	void         _updateParam();

protected:
	std::string          mSensorName;
	Sensor              *mpSensor;
	Recorder             mRecorder;
	int                  mBeatPerMinute;
	int                  mSensorData;
	int                  mBeatPauseTime;
	Cardioid             mCardioid;
	Monitor             *mpMonitor;
	Listener             mListener;

	// params
	ci::params::PInterfaceGl  mParams;
	// recordnig
	std::string               mOutputFileName;
	bool                      mRecording;
	// available devices
	std::vector<std::string>  mDeviceNames;
	std::string               mDummy;
	std::string               mDeviceName;

	// Cardioid
	bool                      mVisible;
	float                     mFactorX;
	float                     mFactorY;
	int                       mOffset;
};


} //namespace HeartRate

