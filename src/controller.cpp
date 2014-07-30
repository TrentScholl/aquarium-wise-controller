#include "controller.h"
#include "relay.h"
#include "fan.h"
#include "configuration.h"
#include "dosing_pump.h"
#include <TimeAlarms.h>
#include <OneWire.h>
#include <DallasTemperature.h>

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

OneWire oneWire(40);

DallasTemperature dallasTemperatureSensors(&oneWire);

DeviceAddress temperatureProbe01 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
DeviceAddress temperatureProbe02 = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };

void Controller::setup()
{
	// Lights
	Alarm.alarmRepeat(12, 00, 0, rly01.on);
	Alarm.alarmRepeat(20, 00, 0, rly01.off);
	
	// Night Lights
	Alarm.alarmRepeat(20, 00, 0, rly02.on);
	Alarm.alarmRepeat(21, 30, 0, rly02.off);

	// CO2
	Alarm.alarmRepeat(11, 00, 0, rly03.on);
	Alarm.alarmRepeat(19, 00, 0, rly03.off);

	dp01.setMlsSec(1);
	dp02.setMlsSec(1);
	dp03.setMlsSec(1);
	
	dp01.setDoseAmt(25);
	dp02.setDoseAmt(25);
	dp03.setDoseAmt(5);
	
	// Macro
	Alarm.alarmRepeat(dowSunday, 11, 00, 0, dp01.dose);
	Alarm.alarmRepeat(dowTuesday, 11, 00, 0, dp01.dose);
	Alarm.alarmRepeat(dowThursday, 11, 00, 0, dp01.dose);
	
	// Micro
	Alarm.alarmRepeat(dowMonday, 11, 00, 0, dp02.dose);
	Alarm.alarmRepeat(dowWednesday, 11, 00, 0, dp02.dose);
	Alarm.alarmRepeat(dowFriday, 11, 00, 0, dp02.dose);
	
	// Glut
	Alarm.alarmRepeat(dowSunday, 20, 00, 0, dp02.dose);
	Alarm.alarmRepeat(dowWednesday, 20, 00, 0, dp02.dose);
	
	dallasTemperatureSensors.begin();
	
	dallasTemperatureSensors.setHighAlarmTemp(temperatureProbe01, 32);
	dallasTemperatureSensors.setLowAlarmTemp(temperatureProbe01, 20);
}

void Controller::loop()
{
	check_Temperatures();
	
	Alarm.delay(0);
}

void Controller::check_Temperatures()
{
	dallasTemperatureSensors.requestTemperatures();
	
	float probeTemp01 = dallasTemperatureSensors.getTempC(temperatureProbe01);
	
	if (dallasTemperatureSensors.hasAlarm(temperatureProbe01))
	{
		if (probeTemp01 >= dallasTemperatureSensors.getHighAlarmTemp(temperatureProbe01) & rly04.getState() == 1)
		{
			rly04.off();
		} 
		else if (probeTemp01 <= dallasTemperatureSensors.getLowAlarmTemp(temperatureProbe01))
		{
			// Sound alarm
		}
	}
	else if (rly04.getState() == 0)
	{
		rly04.on();
	}
	
	float probeTemp02 = dallasTemperatureSensors.getTempC(temperatureProbe02);
}

Controller controller;

