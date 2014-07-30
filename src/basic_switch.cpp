#include "basic_switch.h"

void* basicSwitchObject;

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
	state = LOW;
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
	BasicSwitch* mySelf = (BasicSwitch*)basicSwitchObject;
	mySelf->setState(HIGH);
}

void BasicSwitch::off()
{
	BasicSwitch* mySelf = (BasicSwitch*)basicSwitchObject;
	mySelf->setState(LOW);
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