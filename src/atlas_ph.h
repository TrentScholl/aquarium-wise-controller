#ifndef _ATLAS_PH_h
#define _ATLAS_PH_h

#include "Arduino.h"

class AtlasPh
{
	protected:
		byte highAlarmPh;
		byte lowAlarmPh;
		HardwareSerial *hwSerial;
	public:
		AtlasPh(HardwareSerial *serialPort);
		float requestPh(float tempVal);
		float requestPh();
		void setHighAlarmPh(byte);
		void setLowAlarmPh(byte);
		byte getHighAlarmPh();
		byte getLowAlarmPh();
};

#endif

