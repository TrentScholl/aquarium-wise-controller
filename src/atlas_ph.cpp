#include "atlas_ph.h"
#include <SoftwareSerial.h>

AtlasPh::AtlasPh()
{
	Serial1.begin(38400);
	
	Serial1.print("E\r");
	delay(50);
	Serial1.print("E\r");
	delay(50);
}

float AtlasPh::requestPh(int tempVal)
{
	Serial1.print(tempVal);
	delay(50);

	Serial1.print("R\r");
    delay(300);
	
	float phVal = Serial1.parseFloat();
	
	return phVal;
}

void AtlasPh::setHighAlarmPh(char phVal)
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

void AtlasPh::setLowAlarmPh(char phVal)
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

char AtlasPh::getHighAlarmPh()
{
	return (char)highAlarmPh;
}

char AtlasPh::getLowAlarmPh()
{
	return (char)lowAlarmPh;
}