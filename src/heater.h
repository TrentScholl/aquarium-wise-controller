#ifndef _HEATER_h
#define _HEATER_h

#include <EEPROMex.h>
#include "Arduino.h"
#include "relay.h"

class Heater
{
	protected:
		Relay *relay;
		byte offTemp;
		byte onTemp;
    byte warnTemp;
		int cfgByte;

	public:
		Heater(Relay *rly, int configByte);
    byte getOffTemp();
    void setOffTemp(byte temp);
    byte getOnTemp();
    void setOnTemp(byte temp);
    byte getWarnTemp();
    void setWarnTemp(byte temp);
		void saveSettings();
    void on();
    void off();
    bool isOn();
};

#endif

