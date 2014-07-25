#ifndef _DOSING_PUMP_h
#define _DOSING_PUMP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class DosingPump
{
	protected:
		int pin;
	public:
		DosingPump(int val);
		DosingPump();
		
		int getPin();
		void setPin(int val);
		void dose(int valMl, int valMls);
};

extern DosingPump dosingPump;

#endif

