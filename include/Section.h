#pragma once

#include <string>
#include <sstream>
#include "cinder/Vector.h"
#include "cinder/Font.h"
#include "cinder/Color.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Text.h"

namespace HeartRate
{

#if defined( CINDER_COCOA )
#define MONITOR_FONT ci::Font( "Geneva", 16 )
#else
#define MONITOR_FONT ci::Font( "Tahoma", 16 )
#endif

const ci::ColorA MONITOR_COLOR_BACK        = ci::ColorA( 0.3f, 0.3f, 0.3f, 0.4f );
const ci::ColorA MONITOR_COLOR_FRAME       = ci::ColorA( 0.9f, 0.9f, 0.9f, 0.4f );
const ci::ColorA MONITOR_COLOR_TEXT_TITLE  = ci::ColorA( 0.0f, 1.0f, 0.0f, 1.0f );
const ci::ColorA MONITOR_COLOR_TEXT_HEADER = ci::ColorA( 1.0f, 1.0f, 1.0f, 1.0f );
const ci::ColorA MONITOR_COLOR_TEXT_NAME   = ci::ColorA( 1.0f, 1.0f, 1.0f, 1.0f );
const ci::ColorA MONITOR_COLOR_TEXT_VALUE  = ci::ColorA( 1.0f, 1.0f, 1.0f, 1.0f );

class Section
{
public:
	Section( const std::string name, const ci::ColorA colorName, const ci::ColorA colorValue );
	virtual ~Section();

	virtual void draw     ( ci::Vec2i pos, int width, int heightMax ) = 0;
	virtual int  getHeight(                int width                ) = 0;

protected:
	std::string         mName;
	ci::Font            mFont;
	ci::ColorA          mColorName;
	ci::ColorA          mColorValue;
};

template<class T>
class TSection : public Section
{
public:
	TSection( const std::string name, const T *value, const ci::ColorA colorName = MONITOR_COLOR_TEXT_NAME, const ci::ColorA colorValue = MONITOR_COLOR_TEXT_VALUE );

	void draw     ( ci::Vec2i pos, int width, int heightMax );
	int  getHeight(                int width                );

protected:
	const T *mValue;
};

template<class T>
TSection<T>::TSection( const std::string name, const T *value, const ci::ColorA colorName, const ci::ColorA colorValue )
: Section( name, colorName, colorValue )
, mValue( value )
{
}

template<class T>
void TSection<T>::draw( ci::Vec2i pos, int width, int heightMax )
{
	std::ostringstream valueStream;

	if( mValue )
		valueStream << mName << " - " << *mValue;
	else
		valueStream << mName;

	ci::TextBox label = ci::TextBox().alignment( ci::TextBox::LEFT ).font( mFont ).size( ci::Vec2i( width, ci::TextBox::GROW )).text( valueStream.str());
	label.setColor( mColorName );
	ci::Surface surfaceLabel = label.render();

	if( surfaceLabel.getHeight() <= heightMax )
		ci::gl::draw( surfaceLabel, pos );
}

template<class T>
int TSection<T>::getHeight( int width )
{
	std::ostringstream valueStream;

	if( mValue )
		valueStream << mName << " - " << *mValue;
	else
		valueStream << mName;

	ci::TextBox label = ci::TextBox().alignment( ci::TextBox::LEFT ).font( mFont ).size( ci::Vec2i( width, ci::TextBox::GROW ) ).text( valueStream.str());
	ci::Surface surfaceLabel = label.render();
	return surfaceLabel.getHeight();
}

class SectionFbo : public Section
{
public:
	SectionFbo( const std::string name, ci::gl::Fbo *value, const ci::ColorA colorName = MONITOR_COLOR_TEXT_NAME, const ci::ColorA colorValue = MONITOR_COLOR_TEXT_VALUE );

	void draw     ( ci::Vec2i pos, int width, int heightMax );
	int  getHeight(                int width                );

protected:
	ci::gl::Fbo *mValue;
};


} //namespace HeartRate

