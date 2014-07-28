#include "controller.h"
#include "relay.h"
#include "fan.h"
#include "configuration.h"
#include "dosing_pump.h"

Relay rlyLight1(A0);
Relay rlyLight2(A1);
Relay rlyFilter(A2);
Relay rlyPowerHead(A3);
Relay rlyHeater(A4);
Relay rlyCO2(A5);
Relay rlyAux1(A6);
Relay rlyAux2(A7);

DosingPump dpMacro(10);
DosingPump dpMicro(11);
DosingPump dpFe(12);

int utcOffset=10;

void Controller::setup()
{

}

void Controller::loop()
{

}

Controller controller;

