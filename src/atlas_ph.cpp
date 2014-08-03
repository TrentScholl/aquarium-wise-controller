#include "atlas_ph.h"

AtlasPh::AtlasPh()
{	
}

float AtlasPh::requestPh()
{
	return requestPh(25.00);
}

float AtlasPh::requestPh(float tempVal)
{
	float phVal;
		
	Serial2.print(tempVal);
	Serial2.print("\r");

    while (Serial2.available() > 0) {
	    phVal = Serial2.parseFloat();
		        if (Serial2.read() == '\r') {
			        //Serial.println(phVal);  // can be used for debug, pc display etc.
		        }
    }
	
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