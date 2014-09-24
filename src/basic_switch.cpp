#include "basic_switch.h"
#include "debug.h"

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
	setState(HIGH);
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
	setState(LOW);
}

void BasicSwitch::off()
{
	setState(HIGH);
}

void BasicSwitch::toggle()
{
	if (state==LOW)
	{
		off();
	}
	else
	{
		on();
	}
}

void BasicSwitch::setState(byte val)
{
	if (state != val)
	{
		digitalWrite(pin, val);
		state = val;
	}
}