#pragma once

#include <boost/signals2/signal.hpp>

namespace HeartRate
{

class Listener
{
	typedef boost::signals2::signal<void (int)>      ListenerSignal;
	typedef std::map<int, ListenerSignal*> MapType;

public:
	// Add functions with single integer argument to signal
	template<typename T>
	void addCallback( int type, void (T::* callbackFunction)( int value ), T * callbackObject )
	{
		ListenerSignal *signal = 0;
		MapType::iterator it = mSignals.lower_bound( type );

		if( it != mSignals.end() && !( mSignals.key_comp()( type, it->first )))
		{
			signal = it->second;
		}
		else
		{
			signal = new ListenerSignal;
			mSignals.insert( make_pair( type, signal ));
		}

		signal->connect( boost::function<void ( int )>( boost::bind( callbackFunction, callbackObject, ::_1 )));
	}

	void callCallback( int type, int value )
	{
		MapType::iterator it = mSignals.lower_bound( type );

		if( it != mSignals.end() && !( mSignals.key_comp()( type, it->first )))
		{
			it->second->operator()( value );
		}
	}

protected:
	MapType mSignals;
};

} //namespace HeartRate
