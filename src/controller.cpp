#include "controller.h"
#include "relay.h"
#include "fan.h"
#include "configuration.h"
#include "dosing_pump.h"
#include <TimeAlarms.h>

void* controllerObject;

Relay rly01(A0);
Relay rly02(A1);
Relay rly03(A2);
Relay rly04(A3);
Relay rly05(A4);
Relay rly06(A5);
Relay rly07(A6);
Relay rly08(A7);

DosingPump dp01(10);
DosingPump dp02(11);
DosingPump dp03(12);

void Controller::setup()
{
	Alarm.alarmRepeat(8, 30, 0, Controller::wrapper_rly01_On);
	Alarm.alarmRepeat(8, 31, 0, Controller::wrapper_rly01_Off);
	Alarm.alarmRepeat(8, 30, 0, Controller::wrapper_rly02_On);
	Alarm.alarmRepeat(8, 31, 0, Controller::wrapper_rly02_Off);
	Alarm.alarmRepeat(8, 30, 0, Controller::wrapper_rly03_On);
	Alarm.alarmRepeat(8, 31, 0, Controller::wrapper_rly03_Off);
	Alarm.alarmRepeat(8, 30, 0, Controller::wrapper_rly04_On);
	Alarm.alarmRepeat(8, 31, 0, Controller::wrapper_rly04_Off);
	Alarm.alarmRepeat(8, 30, 0, Controller::wrapper_rly05_On);
	Alarm.alarmRepeat(8, 31, 0, Controller::wrapper_rly05_Off);
	Alarm.alarmRepeat(8, 30, 0, Controller::wrapper_rly06_On);
	Alarm.alarmRepeat(8, 31, 0, Controller::wrapper_rly06_Off);
	Alarm.alarmRepeat(8, 30, 0, Controller::wrapper_rly07_On);
	Alarm.alarmRepeat(8, 31, 0, Controller::wrapper_rly07_Off);
	Alarm.alarmRepeat(8, 30, 0, Controller::wrapper_rly08_On);
	Alarm.alarmRepeat(8, 31, 0, Controller::wrapper_rly08_Off);						
}

void Controller::loop()
{
	Alarm.delay(0);
}

void Controller::rly_State(Relay rly, byte state)
{
	switch (state)
	{
		case 0:
			rly.off();
			break;
		case 1:
			rly.on();
			break;
	}
}

// Relay wrapper functions used for the TimeAlarms

void Controller::wrapper_rly01_On()
{
       Controller* mySelf = (Controller*)controllerObject;
       mySelf->rly_State(rly01, 1);
}

void Controller::wrapper_rly01_Off()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly01, 0);
}

void Controller::wrapper_rly02_On()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly02, 1);
}

void Controller::wrapper_rly02_Off()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly02, 0);
}

void Controller::wrapper_rly03_On()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly03, 1);
}

void Controller::wrapper_rly03_Off()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly03, 0);
}

void Controller::wrapper_rly04_On()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly04, 1);
}

void Controller::wrapper_rly04_Off()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly04, 0);
}

void Controller::wrapper_rly05_On()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly05, 1);
}

void Controller::wrapper_rly05_Off()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly05, 0);
}

void Controller::wrapper_rly06_On()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly06, 1);
}

void Controller::wrapper_rly06_Off()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly06, 0);
}

void Controller::wrapper_rly07_On()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly07, 1);
}

void Controller::wrapper_rly07_Off()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly07, 0);
}

void Controller::wrapper_rly08_On()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly08, 1);
}

void Controller::wrapper_rly08_Off()
{
	Controller* mySelf = (Controller*)controllerObject;
	mySelf->rly_State(rly08, 0);
}

Controller controller;

