#ifndef _DOSING_PUMP_h
#define _DOSING_PUMP_h

#include "Arduino.h"

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
		void dose();
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

