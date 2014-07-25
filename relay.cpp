#include "Relay.h"

Relay::Relay(int val)
{
	setPin(val);
}

Relay::Relay()
{

}

void Relay::setPin(int val)
{
	pin = val;
	pinMode(pin, OUTPUT);
}

int Relay::getPin()
{
	return pin;
}

int Relay::getState()
{
	return state;
}

void Relay::on()
{
	setState(HIGH);
}

void Relay::off()
{
	setState(LOW);
}

void Relay::toggle()
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

void Relay::setState(int val)
{
	digitalWrite(pin, val);
	state = val;
}