#include "basic_switch.h"

BasicSwitch::BasicSwitch(int val)
{
	setPin(val);
}

BasicSwitch::BasicSwitch()
{

}

void BasicSwitch::setPin(int val)
{
	pin = val;
	pinMode(pin, OUTPUT);
}

int BasicSwitch::getPin()
{
	return pin;
}

int BasicSwitch::getState()
{
	return state;
}

void BasicSwitch::on()
{
	setState(HIGH);
}

void BasicSwitch::off()
{
	setState(LOW);
}

void BasicSwitch::toggle()
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

void BasicSwitch::setState(int val)
{
	if (state != val)
	{
		digitalWrite(pin, val);
		state = val;
	}
}