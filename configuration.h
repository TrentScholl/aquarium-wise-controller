#ifndef _CONFIGURATION_h
#define _CONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Configuration
{
 protected:
	int configAddress;
 	struct Data {
	 	int test;
 	};

	
 public:
	Configuration();
	Data data;
	
	void load();
	void save();
};

extern Configuration configuration;

#endif

