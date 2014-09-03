#ifndef _ATLAS_PH_h
#define _ATLAS_PH_h

#include "Arduino.h"

class AtlasPh
{
	protected:
		uint8_t highAlarmPh;
		uint8_t lowAlarmPh;
		HardwareSerial *hwSerial;
	public:
		AtlasPh(HardwareSerial *serialPort);
		float requestPh(float tempVal);
		float requestPh();
		void setHighAlarmPh(char);
		void setLowAlarmPh(char);
		char getHighAlarmPh();
		char getLowAlarmPh();
};

#endif

