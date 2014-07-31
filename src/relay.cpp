#include "relay.h"

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
	setState(HIGH);
	pinMode(pin, OUTPUT);
}

int Relay::getState()
{
	return state;
}

void Relay::on()
{
	setState(LOW);
}

void Relay::off()
{
	setState(HIGH);
}

bool Relay::isOn()
{
	if (state==LOW)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Relay::isOff()
{
	if (state==LOW)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Relay::toggle()
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

void Relay::setState(byte val)
{
	if (state != val)
	{
		digitalWrite(pin, val);
		state = val;
	}
}