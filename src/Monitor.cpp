#include "Monitor.h"
#include "cinder/Text.h"

using namespace ci;
using namespace std;

namespace HeartRate
{

Monitor::Monitor()
{
}

Monitor::Monitor( const string name, const Vec2i &pos, const Vec2i &size, const ColorA colorBack )
: mName( name )
, mArea( pos, Vec2i( pos.x + size.x, pos.y + size.y ))
, mMinWidth( 100 )
, mMinHeight( 70 )
, mColorBack( colorBack )
, mColorFrame( MONITOR_COLOR_FRAME )
, mColorTitle( MONITOR_COLOR_TEXT_HEADER )
, mVisible( true )
, mFont( MONITOR_FONT )
, mMouseState( MS_MOVE )
{
	mCbMouseDown = app::App::get()->registerMouseDown( this, &Monitor::mouseDown );
	mCbMouseDrag = app::App::get()->registerMouseDrag( this, &Monitor::mouseDrag );
}

Monitor::~Monitor()
{
	app::App::get()->unregisterMouseDown( mCbMouseDown );
	app::App::get()->unregisterMouseDrag( mCbMouseDrag );
}

void Monitor::draw()
{
	gl::enableAlphaBlending();

	gl::color( mColorBack );
	gl::drawSolidRect( mArea );
	gl::color( mColorFrame );
	gl::drawStrokedRect( mArea );

	// draw header text
	TextBox label = TextBox().alignment( TextBox::CENTER ).font( mFont ).size( Vec2i( mArea.getWidth(), TextBox::GROW ) ).text( mName );
	label.setColor( mColorTitle );
	Surface surfaceLabel = label.render();
	gl::draw( surfaceLabel, mArea.getUL());

	// draw header line
	Vec2i posHeader1 = mArea.getUL();
	posHeader1.y += surfaceLabel.getHeight();
	Vec2i posHeader2 = posHeader1;
	posHeader1.x += mArea.getWidth();
	gl::drawLine( posHeader1, posHeader2 );

	int heightAll = posHeader1.y;
	for( std::vector<shared_ptr<Section>>::iterator p = mSections.begin(); p != mSections.end(); ++p )
	{
		int heightAct = (*p)->getHeight( mArea.getWidth());
		int heightMax = mArea.y2 - heightAll;

		Vec2i pos = Vec2i( mArea.getX1(), heightAll );
		(*p)->draw( pos, mArea.getWidth(), heightMax );

		heightAll += heightAct;

		if( mArea.y2 < heightAll )
			break;
	}

	// draw header line
	Vec2i posResize1 = mArea.getLR();
	posResize1 -= 2;
	Vec2i posResize2 = posResize1;
	posResize2.y -= 5;
	Vec2i posResize3 = posResize1;
	posResize3.x -= 5;
	gl::drawLine( posResize1, posResize2 );
	gl::drawLine( posResize1, posResize3 );

	gl::disableAlphaBlending();
}

void Monitor::AddSection( std::string name )
{
	Section *pSection = new TSection<int>( name, NULL, MONITOR_COLOR_TEXT_TITLE );
	mSections.push_back( shared_ptr<Section>( pSection ));
}

void Monitor::AddSection( std::string name, bool *pboolValue )
{
	Section *pSection = new TSection<bool>( name, pboolValue );
	mSections.push_back( shared_ptr<Section>( pSection ));
}

void Monitor::AddSection( std::string name, int *pintValue )
{
	Section *pSection = new TSection<int>( name, pintValue );
	mSections.push_back( shared_ptr<Section>( pSection ));
}

void Monitor::AddSection( std::string name, std::string *pstrValue )
{
	Section *pSection = new TSection<std::string>( name, pstrValue );
	mSections.push_back( shared_ptr<Section>( pSection ));
}

void Monitor::AddSection( std::string name, gl::Fbo *pfboValue )
{
	Section *pSection = new SectionFbo( name, pfboValue );
	mSections.push_back( shared_ptr<Section>( pSection ));
}

bool Monitor::mouseDown( ci::app::MouseEvent event )
{
	if( mArea.contains( event.getPos()))
	{
		mPosMouseDrag = event.getPos();

		if( mArea.getLR().distance( mPosMouseDrag ) < 10.0f )
			mMouseState   = MS_RESIZE;
		else
			mMouseState   = MS_MOVE;
	}
	else
	{
		mMouseState   = MS_NOTHING;
	}

	return mMouseState != MS_NOTHING;
}

bool Monitor::mouseDrag( ci::app::MouseEvent event )
{
	switch( mMouseState )
	{
	case MS_NOTHING :
		break;
	case MS_MOVE    :
		{
			Vec2i diff = event.getPos() - mPosMouseDrag;
			mArea += diff;
			mPosMouseDrag = event.getPos();
		}
		break;
	case MS_RESIZE  :
		{
			Vec2i diff = event.getPos() - mPosMouseDrag;
			int width  = math<int>::max( mArea.getWidth()  + diff.x, mMinWidth  );
			int height = math<int>::max( mArea.getHeight() + diff.y, mMinHeight );
			mArea.x2 = mArea.x1 + width;
			mArea.y2 = mArea.y1 + height;
			mPosMouseDrag = event.getPos();
		}
		break;
	}

	return mMouseState != MS_NOTHING;
}

ci::Vec2i Monitor::_getSectionPos()
{
// 	if( mSections.size())
// 	{
// 		shared_ptr<Section> section = mSections[mSections.size()-1];
// 		Vec2i pos = section->getPos();
// 		pos.y += section->getHeight();
// 
// 		return pos;
// 	}
// 	else
// 	{
// 		Vec2i pos = mArea.getUL();
// 		pos.y += 20;
// 
// 		return pos;
//	}

	return mArea.getUL();
}

} // namespace HeartRate
