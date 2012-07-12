#include "cinder/Cinder.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace HeartRate
{

fs::path getFullPath( const string &fileName )
{
	fs::path fullPath( getAssetPath( fileName ));
	if ( fullPath.empty() )
	{
#if defined( CINDER_COCOA )
		fs::path assetPath( getAppPath() / "Contents/Resources/assets" );
#else
		fs::path assetPath( getAppPath() / "../../assets" );
#endif
		fullPath = assetPath / fileName;
		createDirectories( assetPath );
	}

	return fullPath;
}

} // namespace HeartRate
