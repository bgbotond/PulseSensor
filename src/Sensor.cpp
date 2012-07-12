
#include "Sensor.h"

using namespace std;

namespace HeartRate
{

Sensor::Sensor()
: mInited( false )
, mName()
{
}

Sensor::~Sensor()
{
}

const bool Sensor::isInited()
{
	return mInited;
}

const string Sensor::getName()
{
	return mName;
}

} // namespace HeartRate
