#include <Wire.h>
#include <RTClib.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <SoftwareSerial.h>
#include <TimeAlarms.h>
#include <Time.h>
#include <EEPROMVar.h>
#include <EEPROMex.h>
#include <LiquidCrystal.h>
#include "relay.h"
#include "fan.h"
#include "configuration.h"
#include "dosing_pump.h"
#include "atlas_ph.h"
#include "Arduino.h"

#define DEBUG
#include "debug_utils.h"

RTC_DS1307 RTC;

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

AtlasPh ph01();

OneWire oneWire(2);

DallasTemperature dallasTemperatureSensors(&oneWire);

DeviceAddress temperatureProbe01;
DeviceAddress temperatureProbe02;

LiquidCrystal lcd(5, 6, 7, 8, 9, 10);

void setup()
{	
	Serial.begin(9600);
	Wire.begin();
	RTC.begin();
	lcd.begin(16, 2);
	dallasTemperatureSensors.begin();
	
	setSyncProvider(syncProvider);
	
	load_Configuration();
}

void load_Configuration()
{
	set_Defaults();
	setup_Relays();
	setup_DosingPumps();
}

void setup_TemperatureSensors()
{
	dallasTemperatureSensors.getAddress(temperatureProbe01, 0);
	dallasTemperatureSensors.getAddress(temperatureProbe02, 1);
	
	dallasTemperatureSensors.setHighAlarmTemp(temperatureProbe01, configuration.data.temp01HighAlarm);
	dallasTemperatureSensors.setLowAlarmTemp(temperatureProbe01, configuration.data.temp01LowAlarm);
	
	dallasTemperatureSensors.setAlarmHandler(&alarm_temperature);
}

void setup_DosingPumps()
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
			Alarm.alarmRepeat(dowSunday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, alarm_dp01_dose);
		}
		if (configuration.data.dp01Monday)
		{
			Alarm.alarmRepeat(dowMonday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, alarm_dp01_dose);
		}
		if (configuration.data.dp01Tuesday)
		{
			Alarm.alarmRepeat(dowTuesday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, alarm_dp01_dose);
		}
		if (configuration.data.dp01Wednesday)
		{
			Alarm.alarmRepeat(dowWednesday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, alarm_dp01_dose);
		}
		if (configuration.data.dp01Thursday)
		{
			Alarm.alarmRepeat(dowThursday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, alarm_dp01_dose);
		}
		if (configuration.data.dp01Friday)
		{
			Alarm.alarmRepeat(dowFriday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, alarm_dp01_dose);
		}
		if (configuration.data.dp01Saturday)
		{
			Alarm.alarmRepeat(dowSaturday, configuration.data.dp01OnHour, configuration.data.dp01OnMinute, 0, alarm_dp01_dose);
		}
	}
	
	if (configuration.data.dp02Active)
	{
		if (configuration.data.dp02Sunday)
		{
			Alarm.alarmRepeat(dowSunday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, alarm_dp02_dose);
		}
		if (configuration.data.dp02Monday)
		{
			Alarm.alarmRepeat(dowMonday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, alarm_dp02_dose);
		}
		if (configuration.data.dp02Tuesday)
		{
			Alarm.alarmRepeat(dowTuesday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, alarm_dp02_dose);
		}
		if (configuration.data.dp02Wednesday)
		{
			Alarm.alarmRepeat(dowWednesday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, alarm_dp02_dose);
		}
		if (configuration.data.dp02Thursday)
		{
			Alarm.alarmRepeat(dowThursday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, alarm_dp02_dose);
		}
		if (configuration.data.dp02Friday)
		{
			Alarm.alarmRepeat(dowFriday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, alarm_dp02_dose);
		}
		if (configuration.data.dp02Saturday)
		{
			Alarm.alarmRepeat(dowSaturday, configuration.data.dp02OnHour, configuration.data.dp02OnMinute, 0, alarm_dp02_dose);
		}
	}
	
	if (configuration.data.dp03Active)
	{
		if (configuration.data.dp03Sunday)
		{
			Alarm.alarmRepeat(dowSunday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, alarm_dp03_dose);
		}
		if (configuration.data.dp03Monday)
		{
			Alarm.alarmRepeat(dowMonday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, alarm_dp03_dose);
		}
		if (configuration.data.dp03Tuesday)
		{
			Alarm.alarmRepeat(dowTuesday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, alarm_dp03_dose);
		}
		if (configuration.data.dp03Wednesday)
		{
			Alarm.alarmRepeat(dowWednesday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, alarm_dp03_dose);
		}
		if (configuration.data.dp03Thursday)
		{
			Alarm.alarmRepeat(dowThursday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, alarm_dp03_dose);
		}
		if (configuration.data.dp03Friday)
		{
			Alarm.alarmRepeat(dowFriday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, alarm_dp03_dose);
		}
		if (configuration.data.dp03Saturday)
		{
			Alarm.alarmRepeat(dowSaturday, configuration.data.dp03OnHour, configuration.data.dp03OnMinute, 0, alarm_dp03_dose);
		}
	}
}

void setup_Relays()
{	
	// Lights
	if (configuration.data.rly01Active)
	{
		check_RelayScheduleState(rly01, configuration.data.rly01OnHour, configuration.data.rly01OnMinute, configuration.data.rly01OffHour, configuration.data.rly01OffMinute);
		
		Alarm.alarmRepeat(configuration.data.rly01OnHour, configuration.data.rly01OnMinute, 0, alarm_rly01_on);
		Alarm.alarmRepeat(configuration.data.rly01OffHour, configuration.data.rly01OffMinute, 0, alarm_rly01_off);
	}
	
	// Moon Light
	if (configuration.data.rly02Active)
	{
		Alarm.alarmRepeat(configuration.data.rly02OnHour, configuration.data.rly02OnMinute, 0, alarm_rly02_on);
		Alarm.alarmRepeat(configuration.data.rly02OffHour, configuration.data.rly02OffMinute, 0, alarm_rly02_off);
	}

	// CO2
	if (configuration.data.rly03Active)
	{
		Alarm.alarmRepeat(configuration.data.rly03OnHour, configuration.data.rly03OnMinute, 0, alarm_rly03_on);
		Alarm.alarmRepeat(configuration.data.rly03OffHour, configuration.data.rly03OffMinute, 0, alarm_rly03_off);
	}
	
	if (configuration.data.rly04Active)
	{
		Alarm.alarmRepeat(configuration.data.rly04OnHour, configuration.data.rly04OnMinute, 0, alarm_rly04_on);
		Alarm.alarmRepeat(configuration.data.rly04OffHour, configuration.data.rly04OffMinute, 0, alarm_rly04_off);
	}
	
	if (configuration.data.rly05Active)
	{
		Alarm.alarmRepeat(configuration.data.rly05OnHour, configuration.data.rly05OnMinute, 0, alarm_rly05_on);
		Alarm.alarmRepeat(configuration.data.rly05OffHour, configuration.data.rly05OffMinute, 0, alarm_rly05_off);
	}
	
	if (configuration.data.rly06Active)
	{
		Alarm.alarmRepeat(configuration.data.rly06OnHour, configuration.data.rly06OnMinute, 0, alarm_rly06_on);
		Alarm.alarmRepeat(configuration.data.rly06OffHour, configuration.data.rly06OffMinute, 0, alarm_rly06_off);
	}
}

void check_RelayScheduleState(Relay rly, byte onHour, byte onMinute, byte offHour, byte offMinute)
{
	time_t currentTime = tmConvert_t(year(), month(), day(), hour(), minute(), second());
	
	time_t powerOn = tmConvert_t(year(), month(), day(), onHour, onMinute, 0);
	time_t powerOff = tmConvert_t(year(), month(), day(), offHour, offMinute, 0);
	
	if (powerOff < powerOn)
	{
		if (powerOn <= currentTime)
		{
			rly.on();
		}
	}
	else if ((powerOn <= currentTime) && (powerOff > currentTime))
	{
		rly.on();
	}
}

void loop()
{
	check_Temperatures();
	check_Ph();
	
	Alarm.delay(0);
}

void check_Temperatures()
{
	dallasTemperatureSensors.requestTemperatures();
	
	dallasTemperatureSensors.processAlarms();
	
	float probeTemp01 = dallasTemperatureSensors.getTempC(temperatureProbe01);

	if (!dallasTemperatureSensors.hasAlarm(temperatureProbe01) && rly08.isOff())
	{
		rly08.on();
	}

	float probeTemp02 = dallasTemperatureSensors.getTempC(temperatureProbe02);
	
	lcd.setCursor(0, 0);
	lcd.print("Temp:       C");
	lcd.setCursor(11, 0);
	lcd.print((char)223);
	lcd.setCursor(6, 0);
	
	lcd.print(probeTemp01);
}

void check_Ph()
{
	//float phVal = ph01.getPh(dallasTemperatureSensors.getTempC(temperatureProbe01));
	
	//if (phVal >= ph01.getHighAlarmPh() & rly03.getState() == 0)
	//{
		//rly03.on();
	//}
	//else if (phVal <= ph01.getLowAlarmPh() & rly03.getState() == 1)
	//{
		//rly03.off();
	//}
}


// Alarms

void alarm_temperature(const uint8_t* deviceAddress)
{
	float probeTemp = dallasTemperatureSensors.getTempC(deviceAddress);

	if (probeTemp >= dallasTemperatureSensors.getHighAlarmTemp(deviceAddress) & rly08.isOn())
	{
		rly08.off();
	}
	else if (probeTemp <= dallasTemperatureSensors.getLowAlarmTemp(deviceAddress))
	{
		// Sound alarm
	}
}

void alarm_rly01_on()
{
	rly01.on();
}

void alarm_rly01_off()
{
	rly01.off();
}

void alarm_rly02_on()
{
	rly02.on();
}

void alarm_rly02_off()
{
	rly02.off();
}

void alarm_rly03_on()
{
	rly03.on();
}

void alarm_rly03_off()
{
	rly03.off();
}

void alarm_rly04_on()
{
	rly04.on();
}

void alarm_rly04_off()
{
	rly04.off();
}

void alarm_rly05_on()
{
	rly05.on();
}

void alarm_rly05_off()
{
	rly05.off();
}

void alarm_rly06_on()
{
	rly06.on();
}

void alarm_rly06_off()
{
	rly06.off();
}

void alarm_rly07_on()
{
	rly07.on();
}

void alarm_rly07_off()
{
	rly07.off();
}

void alarm_rly08_on()
{
	rly08.on();
}

void alarm_rly08_off()
{
	rly08.off();
}

void alarm_dp01_dose()
{
	dp01.dose();
}

void alarm_dp02_dose()
{
	dp02.dose();
}

void alarm_dp03_dose()
{
	dp03.dose();
}

void set_Defaults()
{
	if (!configuration.data.loaded)
	{
		configuration.data.loaded = 1;
		
		configuration.data.rly01Active = 1;
		configuration.data.rly01OnHour = 11;
		configuration.data.rly01OnMinute = 0;
		configuration.data.rly01OffHour = 19;
		configuration.data.rly01OffMinute = 0;
		
		configuration.data.rly02Active = 1;
		configuration.data.rly02OnHour = 19;
		configuration.data.rly02OnMinute = 0;
		configuration.data.rly02OffHour = 21;
		configuration.data.rly02OffMinute = 0;
		
		configuration.data.rly03Active = 1;
		configuration.data.rly03OnHour = 10;
		configuration.data.rly03OnMinute = 0;
		configuration.data.rly03OffHour = 18;
		configuration.data.rly03OffMinute = 0;
		
		configuration.data.rly04Active = 0;
		configuration.data.rly04OnHour = 12;
		configuration.data.rly04OnMinute = 0;
		configuration.data.rly04OffHour = 23;
		configuration.data.rly04OffMinute = 0;
		
		configuration.data.rly05Active = 0;
		configuration.data.rly05OnHour = 12;
		configuration.data.rly05OnMinute = 0;
		configuration.data.rly05OffHour = 23;
		configuration.data.rly05OffMinute = 0;
		
		configuration.data.rly06Active = 0;
		configuration.data.rly06OnHour = 12;
		configuration.data.rly06OnMinute = 0;
		configuration.data.rly06OffHour = 23;
		configuration.data.rly06OffMinute = 0;
		
		configuration.data.temp01HighAlarm = 32;
		configuration.data.temp01LowAlarm = 20;
		
		configuration.data.dp01Active = 1;
		configuration.data.dp01Capacity = 100;
		configuration.data.dp01Remaining = 0;
		configuration.data.dp01MlSec = 5;
		configuration.data.dp01DoseAmt = 5;
		configuration.data.dp01OnHour = 11;
		configuration.data.dp01OnMinute = 0;
		configuration.data.dp01Sunday = 0;
		configuration.data.dp01Monday = 0;
		configuration.data.dp01Tuesday = 0;
		configuration.data.dp01Wednesday = 0;
		configuration.data.dp01Thursday = 0;
		configuration.data.dp01Friday = 0;
		configuration.data.dp01Saturday = 0;
		
		configuration.data.dp02Active = 1;
		configuration.data.dp02Capacity = 100;
		configuration.data.dp02Remaining = 0;
		configuration.data.dp02MlSec = 5;
		configuration.data.dp02DoseAmt = 5;
		configuration.data.dp02OnHour = 11;
		configuration.data.dp02OnMinute = 0;
		configuration.data.dp02Sunday = 0;
		configuration.data.dp02Monday = 0;
		configuration.data.dp02Tuesday = 0;
		configuration.data.dp02Wednesday = 0;
		configuration.data.dp02Thursday = 0;
		configuration.data.dp02Friday = 0;
		configuration.data.dp02Saturday = 0;
		
		configuration.data.dp03Active = 1;
		configuration.data.dp03Capacity = 100;
		configuration.data.dp02Remaining = 0;
		configuration.data.dp03MlSec = 5;
		configuration.data.dp03DoseAmt = 5;
		configuration.data.dp03OnHour = 11;
		configuration.data.dp03OnMinute = 0;
		configuration.data.dp03Sunday = 0;
		configuration.data.dp03Monday = 0;
		configuration.data.dp03Tuesday = 0;
		configuration.data.dp03Wednesday = 0;
		configuration.data.dp03Thursday = 0;
		configuration.data.dp03Friday = 0;
		configuration.data.dp03Saturday = 0;

		configuration.save();
	}
}

time_t syncProvider()
{
	return RTC.now().unixtime();
}

time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss)
{
	tmElements_t tmSet;
	
	tmSet.Year = YYYY - 1970;
	tmSet.Month = MM;
	tmSet.Day = DD;
	tmSet.Hour = hh;
	tmSet.Minute = mm;
	tmSet.Second = ss;
	
	return makeTime(tmSet);
}