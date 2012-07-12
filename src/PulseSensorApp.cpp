#include "cinder/app/AppBasic.h"

#include "PulseSensor.h"
#include "Util.h"
#include "Monitor.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace HeartRate;

class PulseSensorApp : public AppBasic
{
public:
	void prepareSettings( Settings *settings );
	void setup();
	void shutdown();

	void keyDown( KeyEvent event );
	void resize( ResizeEvent event );
	void mouseDown( MouseEvent event );
	void mouseDrag( MouseEvent event );

	void update();
	void draw();

private:
	PulseSensor mPulseSensor1;
	PulseSensor mPulseSensor2;
};

void PulseSensorApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 640, 480 );
}

void PulseSensorApp::setup()
{
	// params
	fs::path paramsXml = getFullPath( "params.xml" );
	params::PInterfaceGl::load( paramsXml );

	gl::disableVerticalSync();

//	mPulseSensor1.init( "pulsesensor-capture-out-com7.xml" );
	mPulseSensor2.init( "pulsesensor-capture-out-com8.xml" );

//	mPulseSensor1.setup( "Sensor1" );
	mPulseSensor2.setup( "Sensor2" );
}

void PulseSensorApp::shutdown()
{
	params::PInterfaceGl::save();
}

void PulseSensorApp::resize( ResizeEvent event )
{
	mPulseSensor1.resize( event.getWidth(), event.getHeight());
	mPulseSensor2.resize( event.getWidth(), event.getHeight());
}

void PulseSensorApp::keyDown( KeyEvent event )
{
	if ( event.getChar() == 'f' )
		setFullScreen( !isFullScreen() );
	if ( event.getCode() == KeyEvent::KEY_ESCAPE )
		quit();
}

void PulseSensorApp::mouseDown( MouseEvent event )
{
}

void PulseSensorApp::mouseDrag( MouseEvent event )
{
}

void PulseSensorApp::update()
{
	mPulseSensor1.update();
	mPulseSensor2.update();
}

void PulseSensorApp::draw()
{
	gl::clear( Color::black() );
	gl::setMatricesWindow( getWindowSize());

	mPulseSensor1.draw();
	mPulseSensor2.draw();

	params::InterfaceGl::draw();
}

CINDER_APP_BASIC( PulseSensorApp, RendererGl( RendererGl::AA_NONE ) )

