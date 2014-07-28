#include "fan.h"

Fan::Fan(int valPin, int valLowTemp, int valHighTemp)
{
	setPin(valPin);
	setLowTemp(valLowTemp);
	setHighTemp(valHighTemp);
}

void Fan::checkTemp(int val)
{
	if (val > highTemp)
	{
		on();
	}
	else if(val < lowTemp)
	{
		off();
	}
}