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

void DosingPump::setMlSec(int val)
{
	mlSec = val;
}

int DosingPump::getMlSec()
{
	return mlSec;
}

void DosingPump::setDoseAmt(int val)
{
	doseAmt = val;
}

int DosingPump::getDoseAmt()
{
	return doseAmt;
}

void DosingPump::dose()
{
	dose(getDoseAmt());
}

void DosingPump::dose(int val)
{
	analogWrite(pin, 255);
	delay((mlSec*10)*val);
	analogWrite(pin, 0);
}

DosingPump dosingPump;