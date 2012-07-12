#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Fbo.h"

namespace HeartRate
{

class Cardioid
{
public:
	Cardioid();

	void            addData( int data );

	const ci::Rectf getRect();
	void            setRect( const ci::Rectf rect );

	const int       getUnitNum();
	void            setUnitNum( const int unitNum );

	const float     getFactorX();
	void            setFactorX( const float factorX );

	const float     getFactorY();
	void            setFactorY( const float factorY );

	const int       getOffset();
	void            setOffset( const int offset );

	const bool      getVisible();
	void            setVisible( const bool visible );

	ci::gl::Fbo *getFbo();

	void setup();

protected:
	void _updateFbo();

protected:
	ci::gl::Fbo       mFbo;
	ci::Shape2d       mShape;
	std::vector<int>  mData;
	ci::Rectf         mRect;
	float             mFactorX;    // factor to display Data on for x axis
	float             mFactorY;    // factor to display Data on for y axis
	int               mOffset;     // offset to display Data on for y axis
	bool              mVisible;
};


} //namespace HeartRate

