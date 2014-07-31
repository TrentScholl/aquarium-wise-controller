#ifndef _BASICSWITCH_h
#define _BASICSWITCH_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class BasicSwitch
{
	protected:
	int pin;
	byte state;
	void setState(byte val);

	public:
	BasicSwitch(int val);
	BasicSwitch();
	
	int getPin();
	void setPin(int val);
	int getState();
	void on();
	void off();
	void toggle();
};

#endif

