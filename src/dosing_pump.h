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
		int mlsSec;
	public:
		DosingPump(int val);
		DosingPump();
		
		int getPin();
		void setPin(int val);
		int getDoseAmt();
		void setDoseAmt(int val);
		int getMlsSec();
		void setMlsSec(int val);
		static void dose();
		void dose(int val);
};

#endif

