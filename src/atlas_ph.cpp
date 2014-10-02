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

void AtlasPh::setHighAlarmPh(float phVal)
{
	if (phVal > 14)
	{
		phVal = 14;
	}
	else if (phVal < 4)
	{
		phVal = 4;
	}

	highAlarmPh = phVal;
}

void AtlasPh::setLowAlarmPh(float phVal)
{
	if (phVal > 14)
	{
		phVal = 14;
	}
	else if (phVal < 4)
	{
		phVal = 4;
	}

	lowAlarmPh = phVal;
}

float AtlasPh::getHighAlarmPh()
{
	return (char)highAlarmPh;
}

float AtlasPh::getLowAlarmPh()
{
	return (char)lowAlarmPh;
}