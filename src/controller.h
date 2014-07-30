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
	void load_Configuration();
	void setup_Relays();
	void setup_DosingPumps();
	void loop();
	void check_Temperatures();
	void check_Ph();
};

extern Controller controller;

#endif

