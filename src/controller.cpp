#include "controller.h"
#include "relay.h"
#include "fan.h"
#include "configuration.h"
#include "dosing_pump.h"
#include "atlas_ph.h"
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

AtlasPh ph01(14, 15);

OneWire oneWire(40);

DallasTemperature dallasTemperatureSensors(&oneWire);

DeviceAddress temperatureProbe01 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
DeviceAddress temperatureProbe02 = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };

void Controller::setup()
{
	load_Configuration();
	
	dallasTemperatureSensors.begin();
	
	dallasTemperatureSensors.setHighAlarmTemp(temperatureProbe01, 32);
	dallasTemperatureSensors.setLowAlarmTemp(temperatureProbe01, 20);
}

void Controller::load_Configuration()
{
	configuration.load();
	setup_Relays();
	setup_DosingPumps();
}

void Controller::setup_DosingPumps()
{
	dp01.setMlSec(configuration.data.dp01MlSec);
	dp02.setMlSec(configuration.data.dp02MlSec);
	dp03.setMlSec(configuration.data.dp02MlSec);
		
	dp01.setDoseAmt(configuration.data.dp01DoseAmt);
	dp02.setDoseAmt(configuration.data.dp02DoseAmt);
	dp03.setDoseAmt(configuration.data.dp03DoseAmt);
	
	if (configuration.data.dp01Active)
	{
		if (configuration.data.dp01Sunday)
		{
			Alarm.alarmRepeat(dowSunday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, dp01.dose);
		}
		if (configuration.data.dp01Monday)
		{
			Alarm.alarmRepeat(dowMonday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, dp01.dose);
		}
		if (configuration.data.dp01Tuesday)
		{
			Alarm.alarmRepeat(dowTuesday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, dp01.dose);
		}
		if (configuration.data.dp01Wednesday)
		{
			Alarm.alarmRepeat(dowWednesday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, dp01.dose);
		}
		if (configuration.data.dp01Thursday)
		{
			Alarm.alarmRepeat(dowThursday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, dp01.dose);
		}
		if (configuration.data.dp01Friday)
		{
			Alarm.alarmRepeat(dowFriday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, dp01.dose);
		}
		if (configuration.data.dp01Saturday)
		{
			Alarm.alarmRepeat(dowSaturday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, dp01.dose);
		}
	}
	
	if (configuration.data.dp02Active)
	{
		if (configuration.data.dp02Sunday)
		{
			Alarm.alarmRepeat(dowSunday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, dp02.dose);
		}
		if (configuration.data.dp02Monday)
		{
			Alarm.alarmRepeat(dowMonday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, dp02.dose);
		}
		if (configuration.data.dp02Tuesday)
		{
			Alarm.alarmRepeat(dowTuesday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, dp02.dose);
		}
		if (configuration.data.dp02Wednesday)
		{
			Alarm.alarmRepeat(dowWednesday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, dp02.dose);
		}
		if (configuration.data.dp02Thursday)
		{
			Alarm.alarmRepeat(dowThursday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, dp02.dose);
		}
		if (configuration.data.dp02Friday)
		{
			Alarm.alarmRepeat(dowFriday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, dp02.dose);
		}
		if (configuration.data.dp02Saturday)
		{
			Alarm.alarmRepeat(dowSaturday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, dp02.dose);
		}
	}
	
	if (configuration.data.dp03Active)
	{
		if (configuration.data.dp03Sunday)
		{
			Alarm.alarmRepeat(dowSunday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, dp03.dose);
		}
		if (configuration.data.dp03Monday)
		{
			Alarm.alarmRepeat(dowMonday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, dp03.dose);
		}
		if (configuration.data.dp03Tuesday)
		{
			Alarm.alarmRepeat(dowTuesday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, dp03.dose);
		}
		if (configuration.data.dp03Wednesday)
		{
			Alarm.alarmRepeat(dowWednesday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, dp03.dose);
		}
		if (configuration.data.dp03Thursday)
		{
			Alarm.alarmRepeat(dowThursday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, dp03.dose);
		}
		if (configuration.data.dp03Friday)
		{
			Alarm.alarmRepeat(dowFriday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, dp03.dose);
		}
		if (configuration.data.dp03Saturday)
		{
			Alarm.alarmRepeat(dowSaturday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, dp03.dose);
		}
	}
}

void Controller::setup_Relays()
{
	// Lights
	if (configuration.data.rly01Active)
	{
		Alarm.alarmRepeat(configuration.data.rly01OnHour, configuration.data.rly01OnMinute, 0, rly01.on);
		Alarm.alarmRepeat(configuration.data.rly01OffHour, configuration.data.rly01OffMinute, 0, rly01.off);
	}
	
	// Moon Light
	if (configuration.data.rly02Active)
	{
		Alarm.alarmRepeat(configuration.data.rly02OnHour, configuration.data.rly02OnMinute, 0, rly02.on);
		Alarm.alarmRepeat(configuration.data.rly02OffHour, configuration.data.rly02OffMinute, 0, rly02.off);
	}

	// CO2
	if (configuration.data.rly03Active)
	{
		Alarm.alarmRepeat(configuration.data.rly03OnHour, configuration.data.rly03OnMinute, 0, rly03.on);
		Alarm.alarmRepeat(configuration.data.rly03OffHour, configuration.data.rly03OffMinute, 0, rly03.off);
	}
	
	if (configuration.data.rly04Active)
	{
		Alarm.alarmRepeat(configuration.data.rly04OnHour, configuration.data.rly04OnMinute, 0, rly04.on);
		Alarm.alarmRepeat(configuration.data.rly04OffHour, configuration.data.rly04OffMinute, 0, rly04.off);
	}
	
	if (configuration.data.rly05Active)
	{
		Alarm.alarmRepeat(configuration.data.rly05OnHour, configuration.data.rly05OnMinute, 0, rly05.on);
		Alarm.alarmRepeat(configuration.data.rly05OffHour, configuration.data.rly05OffMinute, 0, rly05.off);
	}
	
	if (configuration.data.rly06Active)
	{
		Alarm.alarmRepeat(configuration.data.rly06OnHour, configuration.data.rly06OnMinute, 0, rly06.on);
		Alarm.alarmRepeat(configuration.data.rly06OffHour, configuration.data.rly06OffMinute, 0, rly06.off);
	}	
}

void Controller::loop()
{
	check_Temperatures();
	check_Ph();
	
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

void Controller::check_Ph()
{
	float phVal = ph01.getPh(dallasTemperatureSensors.getTempC(temperatureProbe01));
	
	if (phVal >= ph01.getHighAlarmPh() & rly03.getState() == 0)
	{
		rly03.on();
	}
	else if (phVal <= ph01.getLowAlarmPh() & rly03.getState() == 1)
	{
		rly03.off();
	}
}

Controller controller;

