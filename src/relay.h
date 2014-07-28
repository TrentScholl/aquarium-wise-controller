#ifndef _RELAY_h
#define _RELAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "basic_switch.h"

class Relay: public BasicSwitch
{
 protected:

 public:
	Relay(int val) : BasicSwitch(val) {}
};

#endif

