#include "fan.h"

Fan::Fan(int valPin, int valLowTemp, int valHighTemp)
{
	setPin(valPin);
	setLowTemp(valLowTemp);
	setHighTemp(valHighTemp);
}

Fan::Fan()
{

}

void Fan::setPin(int val)
{
	pin = val;
	pinMode(pin, OUTPUT);
}

int Fan::getPin()
{
	return pin;
}

int Fan::getState()
{
	return state;
}

void Fan::on()
{
	setState(HIGH);
}

void Fan::off()
{
	setState(LOW);
}

void Fan::toggle()
{
	if (state==HIGH)
	{
		off();
	}
	else
	{
		on();
	}
}

void Fan::setState(int val)
{
	if (state != val)
	{
		digitalWrite(pin, val);
		state = val;
	}
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