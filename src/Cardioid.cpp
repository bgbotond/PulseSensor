
#include "cinder/gl/gl.h"
#include "cinder/CinderMath.h"
#include "Cardioid.h"

using namespace ci;
using namespace std;

namespace HeartRate
{

static const int FBO_WIDTH  = 640;
static const int FBO_HEIGHT = 500;

Cardioid::Cardioid()
: mFbo()
, mFactorX( 1.0 )
, mFactorY( 1.0 )
, mOffset( 600 )
, mVisible( true )
{
}

void Cardioid::addData( int data )
{
	mData.push_back( data );
	_updateFbo();
}

const float Cardioid::getFactorX()
{
	return mFactorX;
}

void Cardioid::setFactorX( const float factorX )
{
	mFactorX = math<float>::min( math<float>::max( factorX, 0.01f ), 1.0f );
}

const float Cardioid::getFactorY()
{
	return mFactorY;
}

void Cardioid::setFactorY( const float factorY )
{
	mFactorY = math<float>::min( math<float>::max( factorY, 0.01f ), 1.0f );
}

const int Cardioid::getOffset()
{
	return mOffset;
}

void Cardioid::setOffset( const int offset )
{
	mOffset = math<int>::min( math<int>::max( offset, 0 ), 900 );
}

const bool Cardioid::getVisible()
{
	return mVisible;
}

void Cardioid::setVisible( const bool visible )
{
	mVisible = visible;
}

ci::gl::Fbo *Cardioid::getFbo()
{
	return &mFbo;
}

void Cardioid::_updateFbo()
{
	mFbo.bindFramebuffer();

	gl::setMatricesWindow( Vec2i( FBO_WIDTH, FBO_HEIGHT ));
	gl::clear( Color::black());

	if( getVisible())
	{
		gl::color( ColorA( 1, 1, 1, 0.8f ));

		int width  = mFbo.getWidth();
		int height = mFbo.getHeight();
		float unitSize = (float)width / ( width * mFactorX );
		unitSize = math<float>::min( math<float>::max( unitSize, 1.0f ), (float)width );
		float posX     = (float)width;
		float posY     = (float)height;
		Vec2f posPrev;
		Vec2f posAct;

		glLineWidth( 2.0 );

		int size = (int)mData.size();
		int pos = size - 1;
		while( posX > 0 && pos >= 0 )
		{
			int data = mData[pos];
			posAct = Vec2f( posX, posY - data * mFactorY + mOffset );

			if( pos != size - 1 )
				gl::drawLine( posAct, posPrev );

			posPrev = posAct;

			pos--;
			posX -= unitSize;
		}

		if( posX )
		{
			posAct = Vec2f( --posX, posY + mOffset );
			gl::drawLine( posAct, posPrev );
			posPrev = posAct;
			posAct = Vec2f( 0, posY + mOffset );
			gl::drawLine( posAct, posPrev );
		}

		glLineWidth( 1.0 );
	}

	mFbo.unbindFramebuffer();
}

void Cardioid::setup()
{
	gl::Fbo::Format format;
	format.enableDepthBuffer( false );

	mFbo = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format ); 
}

} // namespace HeartRate
