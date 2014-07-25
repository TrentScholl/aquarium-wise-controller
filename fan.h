// fan.h

#ifndef _FAN_h
#define _FAN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Fan
{
 protected:
	 int pin;
	 int state;
	 int highTemp;
	 int lowTemp;
	 void setState(int val);

 public:
	 Fan(int valPin, int valLowTemp, int valHighTemp);
	 Fan();
	 
	 int getPin();
	 void setPin(int val);
	 int getState();
	 void on();
	 void off();
	 void toggle();
	 void checkTemp(int val);
	 void setHighTemp(int val);
	 int getHighTemp();
	 void setLowTemp(int val);
	 int getLowTemp();
};

extern Fan fan;

#endif

