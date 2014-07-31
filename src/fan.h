#ifndef _FAN_h
#define _FAN_h

#include "Arduino.h"

#include "basic_switch.h"

class Fan: public BasicSwitch
{
 protected:
	 int highTemp;
	 int lowTemp;

 public:
	 Fan(int valPin, int valLowTemp, int valHighTemp);

	 void checkTemp(int val);
	 void setHighTemp(int val);
	 int getHighTemp();
	 void setLowTemp(int val);
	 int getLowTemp();
};

#endif

