#ifndef _CONFIGURATION_h
#define _CONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Time.h>
#include <RTClib.h>
#include <TimeAlarms.h>

class Configuration
{
 protected:
	int configAddress;
 	struct Data {
	 	int utcOffset;
		time_t feedTime;
 	};

	
 public:
	Configuration();
	Data data;
	
	void load();
	void save();
};

extern Configuration configuration;

#endif

