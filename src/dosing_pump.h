#ifndef _DOSING_PUMP_h
#define _DOSING_PUMP_h

#include "debug.h"
#include <Time.h>
#include <TimeAlarms.h>
#include <EEPROMex.h>
#include "Arduino.h"

typedef void (*DoseHandler)();

class DosingPump
{
	protected:
		byte pin;
		byte doseAmt;
		int mlSec;
		int vol;
    int remainingVol;
		int cfgByte;
    int alarmIds[6];

	public:
		DosingPump(byte pumpPin, int configByte, DoseHandler handler);
		byte getDoseAmt();
		void setDoseAmt(byte val);
		int getMlSec();
		void setMlSec(int val);
		int getVol();
		void setVol(int val);
    void setRemainingVol(int val);
    int getRemainingVol();
		void dose();
		void saveSettings();
    void updateAlarms();

    DoseHandler doseHandler;
		
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
    Schedule schedule;
};

#endif

