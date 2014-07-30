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
		int doseAmt;
		int mlSec;
	public:
		DosingPump(int val);
		DosingPump();
		
		int getPin();
		void setPin(int val);
		int getDoseAmt();
		void setDoseAmt(int val);
		int getMlSec();
		void setMlSec(int val);
		static void dose();
		void dose(int val);
		struct Schedule
		{
			byte active;
			byte onHour;
			byte onMinute;
			byte Sunday;
			byte Monday;
			byte Tuesday;
			byte Wednesday;
			byte Thursday;
			byte Friday;
			byte Saturday;
		};
};

#endif

