#include "dosing_pump.h"

DosingPump::DosingPump(int val)
{
	setPin(val);
}

DosingPump::DosingPump()
{

}

void DosingPump::setPin(int val)
{
	pin = val;
	pinMode(pin, OUTPUT);
}

int DosingPump::getPin()
{
	return pin;
}

void DosingPump::dose(int valMl, int valMls)
{
	analogWrite(pin, 255);
	delay((valMls*10)*valMl);
	analogWrite(pin, 0);
}

DosingPump dosingPump;