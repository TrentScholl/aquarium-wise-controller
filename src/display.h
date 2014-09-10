#ifndef _DISPLAY_h
#define _DISPLAY_h

#include <EEPROMex.h>
#include "Arduino.h"

class Display
{
	protected:
		byte homeTimeout;
		byte dimLevel;
		byte dimSecs;
    byte brightness;
		int cfgByte;

	public:
		Display(int configByte);
    byte getHomeTimeout();
    void setHomeTimeout(byte timeout);
    byte getDimLevel();
    void setDimLevel(byte level);
    byte getDimSecs();
    void setDimSecs(byte secs);
    byte getBrightness();
    void setBrightness(byte bright);
		void saveSettings();
};

#endif

