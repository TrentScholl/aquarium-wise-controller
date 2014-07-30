#ifndef _CONTROLLER_h
#define _CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "relay.h"

class Controller
{
 private:
	
 public:
	void setup();
	void loop();
	void check_Temperatures();
	void check_Ph();
};

extern Controller controller;

#endif

