#pragma once

#include <string>
#include "cinder/Vector.h"
#include "cinder/gl/Fbo.h"
#include "cinder/App/App.h"
#include "Section.h"

namespace HeartRate
{

class Monitor
{
	enum MouseState
	{
		MS_NOTHING,
		MS_MOVE,
		MS_RESIZE
	};

public:
	Monitor();
	Monitor( const std::string name, const ci::Vec2i &pos, const ci::Vec2i &size = ci::Vec2i( 190, 330 ), const ci::ColorA color = MONITOR_COLOR_BACK );
	~Monitor();

	void draw();

	void AddSection( std::string name                          );
	void AddSection( std::string name, bool        *pboolValue );
	void AddSection( std::string name, int         *pintValue  );
	void AddSection( std::string name, std::string *pstrValue  );
	void AddSection( std::string name, ci::gl::Fbo *pfboValue  );

	bool mouseDown( ci::app::MouseEvent event );
	bool mouseDrag( ci::app::MouseEvent event );

private:
	ci::Vec2i _getSectionPos();

protected:
	std::string                           mName;
	ci::Area                              mArea;
	ci::ColorA                            mColorBack;
	ci::ColorA                            mColorFrame;
	ci::ColorA                            mColorTitle;

	int                                   mMinWidth;
	int                                   mMinHeight;

	bool                                  mVisible;
	ci::Font                              mFont;
	std::vector<std::shared_ptr<Section>> mSections;

	ci::CallbackId                        mCbMouseDown;
	ci::CallbackId                        mCbMouseDrag;
	MouseState                            mMouseState;
	ci::Vec2i                             mPosMouseDrag;
};


} //namespace HeartRate

