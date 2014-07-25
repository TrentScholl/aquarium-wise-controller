#include "controller.h"
#include "relay.h"
#include "fan.h"
#include "configuration.h"

Relay rlyLight1(A0);
Relay rlyLight2(A1);
Relay rlyFilter(A2);
Relay rlyPowerHead(A3);
Relay rlyHeater(A4);
Relay rlyCO2(A5);
Relay rlyAux1(A6);
Relay rlyAux2(A7);

void Controller::setup()
{
	int i = configuration.data.test;
}

void Controller::loop()
{

}

Controller controller;

