#include "atlas_ph.h"

AtlasPh::AtlasPh(HardwareSerial *serialPort)
{	
	hwSerial = serialPort;
	hwSerial->begin(38400);
}

float AtlasPh::requestPh()
{
	return requestPh(25.00);
}

float AtlasPh::requestPh(float tempVal)
{
	float phVal = 0.0;
		
	hwSerial->print(tempVal);
	hwSerial->print("\r");

  while (hwSerial->available() > 0) {
	  phVal = hwSerial->parseFloat();
		if (hwSerial->read() == '\r')
		{
			//Serial.println(phVal); 
		}
  }
	
	return phVal;
}

void AtlasPh::setHighAlarmPh(byte phVal)
{
	if (phVal > 8)
	{
		phVal = 8;
	}
	else if (phVal < 4)
	{
		phVal = 4;
	}

	highAlarmPh = (uint8_t)phVal;
}

void AtlasPh::setLowAlarmPh(byte phVal)
{
	if (phVal > 8)
	{
		phVal = 8;
	}
	else if (phVal < 4)
	{
		phVal = 4;
	}

	lowAlarmPh = (uint8_t)phVal;
}

byte AtlasPh::getHighAlarmPh()
{
	return (char)highAlarmPh;
}

byte AtlasPh::getLowAlarmPh()
{
	return (char)lowAlarmPh;
}