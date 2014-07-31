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
	int state;
	int pin;
	void setState(byte val);

 public:
	Relay(int val);
	void on();
	void off();
	void toggle();
	int getState();
	bool isOn();
	bool isOff();
		  
	struct Schedule
	{
		byte active;
		byte onHour;
		byte onMinute;
		byte offHour;
		byte offMinute;
	};
};

#endif

