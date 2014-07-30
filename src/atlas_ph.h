#ifndef _ATLAS_PH_h
#define _ATLAS_PH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SoftwareSerial.h>

class AtlasPh
{
 protected:
	SoftwareSerial *atlasSerial;
	uint8_t highAlarmPh;
	uint8_t lowAlarmPh;
 public:
	AtlasPh(int rxPin, int txPin);
	float getPh(int tempVal);
	void setHighAlarmPh(char);
	void setLowAlarmPh(char);
	char getHighAlarmPh();
	char getLowAlarmPh();
};

#endif

