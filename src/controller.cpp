#include "controller.h"
#include "relay.h"
#include "fan.h"
#include "configuration.h"
#include "dosing_pump.h"
#include <TimeAlarms.h>

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
	Alarm.alarmRepeat(8, 30, 0, rly01.on);
	Alarm.alarmRepeat(8, 31, 0, rly01.off);
	Alarm.alarmRepeat(8, 30, 0, rly02.on);
	Alarm.alarmRepeat(8, 31, 0, rly02.off);
	Alarm.alarmRepeat(8, 30, 0, rly03.on);
	Alarm.alarmRepeat(8, 31, 0, rly03.off);
	Alarm.alarmRepeat(8, 30, 0, rly04.on);
	Alarm.alarmRepeat(8, 31, 0, rly04.off);
	Alarm.alarmRepeat(8, 30, 0, rly05.on);
	Alarm.alarmRepeat(8, 31, 0, rly05.off);
	Alarm.alarmRepeat(8, 30, 0, rly06.on);
	Alarm.alarmRepeat(8, 31, 0, rly06.off);
	Alarm.alarmRepeat(8, 30, 0, rly07.on);
	Alarm.alarmRepeat(8, 31, 0, rly07.off);
	Alarm.alarmRepeat(8, 30, 0, rly08.on);
	Alarm.alarmRepeat(8, 31, 0, rly08.off);
	
	Alarm.alarmRepeat(8, 31, 0, dp01.dose);
}

void Controller::loop()
{
	Alarm.delay(0);
}

Controller controller;

