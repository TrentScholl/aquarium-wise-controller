#ifndef _RELAY_h
#define _RELAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Relay
{
 protected:
	int pin;
	int state;
	void setState(int val);

 public:
	Relay(int val);
	Relay();
	
	int getPin();
	void setPin(int val);
	int getState();
	void on();
	void off();
	void toggle();
};

#endif

