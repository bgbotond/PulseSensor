#include <fstream>

#include "cinder/app/AppBasic.h"
#include "Recorder.h"
#include "Util.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define USE_XML 1

namespace HeartRate
{

Recorder::Recorder()
: mXmlTree()
, mpXmlFile( 0 )
, mPath()
, mRecording( false )
{
}

void Recorder::startRecording( const std::string &filename, const std::string &name )
{
	mTime      = getElapsedSeconds();
	mRecording = true;
	mPath      = getFullPath( filename );

#if USE_XML == 1
	mXmlTree   = XmlTree( "PulseSensor", "" );
	mXmlTree.setAttribute( "Name", name );
#else
	mpXmlFile = new ofstream( mPath.c_str());
	*mpXmlFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	*mpXmlFile << "<!-- ======================================== -->\n";
	*mpXmlFile << "<!--  Pulse Sensor capture data               -->\n";
	*mpXmlFile << "<!--  --------------------------------------- -->\n";
	*mpXmlFile << "<!--  Time (double): time of input in seconds -->\n";
	*mpXmlFile << "<!--  Type (char)  : type of input            -->\n";
	*mpXmlFile << "<!--  Data (string): input                    -->\n";
	*mpXmlFile << "<!-- ======================================== -->\n";
	*mpXmlFile << "<PulseSensor Name = \"" << name << "\">\n";
#endif
}

void Recorder::stopRecording()
{
#if USE_XML == 1
	XmlTree doc = XmlTree::createDoc();
	XmlTree comment;

	comment = XmlTree( "", " ======================================== ", 0, XmlTree::NODE_COMMENT ); doc.push_back( comment );
	comment = XmlTree( "", "  Pulse Sensor capture data               ", 0, XmlTree::NODE_COMMENT ); doc.push_back( comment );
	comment = XmlTree( "", "  --------------------------------------- ", 0, XmlTree::NODE_COMMENT ); doc.push_back( comment );
	comment = XmlTree( "", "  Time (double): time of input in seconds ", 0, XmlTree::NODE_COMMENT ); doc.push_back( comment );
	comment = XmlTree( "", "  Type (char)  : type of input            ", 0, XmlTree::NODE_COMMENT ); doc.push_back( comment );
	comment = XmlTree( "", "  Data (string): input                    ", 0, XmlTree::NODE_COMMENT ); doc.push_back( comment );
	comment = XmlTree( "", " ======================================== ", 0, XmlTree::NODE_COMMENT ); doc.push_back( comment );

	doc.push_back( mXmlTree );

	ofstream xmlFile;
	xmlFile.open( mPath.c_str());
	xmlFile << doc;
	xmlFile.close();
#else
	*mpXmlFile << "</PulseSensor>\n";
	mpXmlFile->close();
	delete mpXmlFile;
#endif
	mXmlTree   = XmlTree();
	mPath      = fs::path();
	mRecording = false;
	mTime      = 0;
}

bool Recorder::isRecording()
{
	return mRecording;
}

void Recorder::dataRecording( const double time, const char messageId, const string &message )
{
	if( ! isRecording())
		return;

#if USE_XML == 1
	XmlTree data = XmlTree( "Data", "" );
	data.setAttribute( "Time", time - mTime          );
	data.setAttribute( "Type", messageId             );
	data.setAttribute( "Data", atoi( message.c_str()));

	mXmlTree.push_back( data );
#else
	*mpXmlFile << "  <Data  Time = \"" << time - mTime << "\" Type = \"" << messageId << "\" Data = \"" << atoi( message.c_str()) << "\"/>\n";
#endif
}

void Recorder::testRecording()
{
	startRecording( "output-test.xml", "test" );

	dataRecording( 1.5, 'Q', "123" );
	dataRecording( 2.5, 'S', "423" );
	dataRecording( 3.5, 'B', "345" );
	dataRecording( 4.5, 'Q', "341" );
	dataRecording( 5.5, 'S', "456" );
	dataRecording( 6.5, 'B', "234" );

	stopRecording();
}

} // namespace HeartRate
