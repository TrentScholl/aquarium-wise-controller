#ifndef _ATLAS_PH_h
#define _ATLAS_PH_h

#include "Arduino.h"

class AtlasPh
{
	protected:
		float highAlarmPh;
		float lowAlarmPh;
		HardwareSerial *hwSerial;
	public:
		AtlasPh(HardwareSerial *serialPort);
		float requestPh(float tempVal);
		float requestPh();
		void setHighAlarmPh(float);
		void setLowAlarmPh(float);
		float getHighAlarmPh();
		float getLowAlarmPh();
};

#endif

