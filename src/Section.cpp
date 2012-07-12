#include "Section.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Text.h"
#include "cinder/Area.h"

using namespace ci;
using namespace std;

namespace HeartRate
{

Section::Section( const string name, const ColorA colorName, const ColorA colorValue )
: mName( name )
, mFont( MONITOR_FONT )
, mColorName( colorName )
, mColorValue( colorValue )
{
}

Section::~Section()
{
}

SectionFbo::SectionFbo( const string name, gl::Fbo *value, const ColorA colorName, const ColorA colorValue )
: Section( name, colorName, colorValue )
, mValue( value )
{
}

int SectionFbo::getHeight( int width )
{
	int widthOrig  = mValue->getWidth();
	int heightOrig = mValue->getHeight();
	int heigthNew  = (int)((float)width * heightOrig / widthOrig);

	return heigthNew;
}

void SectionFbo::draw( Vec2i pos, int width, int heightMax )
{
	int height = math<int>::min( getHeight( width ), heightMax );
	Area area = Area( pos, Vec2i( pos.x + width, pos.y + height ));

	gl::color( MONITOR_COLOR_FRAME );
	gl::drawStrokedRect( area );

	glEnable( GL_TEXTURE_2D );
	mValue->bindTexture();
	area.x2--;
	area.y1++;
	gl::draw( mValue->getTexture(0), area );
	mValue->unbindTexture();
}

} // namespace HeartRate
