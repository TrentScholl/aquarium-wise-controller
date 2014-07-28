#ifndef _CONTROLLER_h
#define _CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Controller
{
 private:

 public:
	void setup();
	void loop();
};

extern Controller controller;

#endif

