#include "dosing_pump.h"

DosingPump::DosingPump(byte pin, int configByte, DoseHandler handler)
{
	cfgByte = configByte;
  doseHandler = handler;
  
	pinMode(pin, OUTPUT);
	setDoseAmt(EEPROM.readByte(cfgByte + 10));
	setMlSec(EEPROM.readByte(cfgByte + 11) * 10);
	setVol(EEPROM.readByte(cfgByte + 12) * 10);

  schedule.active = EEPROM.readByte(cfgByte);
  schedule.onHour = EEPROM.readByte(cfgByte + 1);
  schedule.onMinute = EEPROM.readByte(cfgByte + 2);
  schedule.Sunday = EEPROM.readByte(cfgByte + 3);
  schedule.Monday = EEPROM.readByte(cfgByte + 4);
  schedule.Tuesday = EEPROM.readByte(cfgByte + 5);
  schedule.Wednesday = EEPROM.readByte(cfgByte + 6);
  schedule.Thursday = EEPROM.readByte(cfgByte + 7);
  schedule.Friday = EEPROM.readByte(cfgByte + 8);
  schedule.Saturday = EEPROM.readByte(cfgByte + 9);
  
  updateAlarms();
}

void DosingPump::updateAlarms()
{
  for (int i = 0; i < 6; i = i++)
  {
    Alarm.free(alarmIds[i]);
  }
  
  if (schedule.active)
  {
    byte onHour = schedule.onHour;
    byte onMinute = schedule.onMinute;
    
    if (schedule.Sunday)
    {
      alarmIds[0] = Alarm.alarmRepeat(dowSunday, onHour, onMinute, 0, doseHandler);
    }
    if (schedule.Monday)
    {
      alarmIds[1] = Alarm.alarmRepeat(dowMonday, onHour, onMinute, 0, doseHandler);
    }
    if (schedule.Tuesday)
    {
      alarmIds[2] = Alarm.alarmRepeat(dowTuesday, onHour, onMinute, 0, doseHandler);
    }
    if (schedule.Wednesday)
    {
      alarmIds[3] = Alarm.alarmRepeat(dowWednesday, onHour, onMinute, 0, doseHandler);
    }
    if (schedule.Thursday)
    {
      alarmIds[4] = Alarm.alarmRepeat(dowThursday, onHour, onMinute, 0, doseHandler);
    }
    if (schedule.Friday)
    {
      alarmIds[5] = Alarm.alarmRepeat(dowFriday, onHour, onMinute, 0, doseHandler);
    }
    if (schedule.Saturday)
    {
      alarmIds[6] = Alarm.alarmRepeat(dowSaturday, onHour, onMinute, 0, doseHandler);
    }
  }
}

void DosingPump::setMlSec(int val)
{
	mlSec = val;
}

int DosingPump::getMlSec()
{
	return mlSec;
}

void DosingPump::setVol(int val)
{
	vol = val;
}

int DosingPump::getVol()
{
	return vol;
}

void DosingPump::setDoseAmt(byte val)
{
	doseAmt = val;
}

byte DosingPump::getDoseAmt()
{
	return doseAmt;
}

void DosingPump::dose()
{
	analogWrite(pin, 255);
	delay(mlSec * doseAmt);
	analogWrite(pin, 0);
}

void DosingPump::saveSettings()
{
  EEPROM.updateByte(cfgByte + 10, doseAmt);
  EEPROM.updateByte(cfgByte + 11, mlSec / 10);
  EEPROM.updateByte(cfgByte + 12, vol / 10);
  EEPROM.updateByte(cfgByte, schedule.active);
  EEPROM.updateByte(cfgByte + 1, schedule.onHour);
  EEPROM.updateByte(cfgByte + 2, schedule.onMinute);
  EEPROM.updateByte(cfgByte + 3, schedule.Sunday);
  EEPROM.updateByte(cfgByte + 4, schedule.Monday);
  EEPROM.updateByte(cfgByte + 5, schedule.Tuesday);
  EEPROM.updateByte(cfgByte + 6, schedule.Wednesday);
  EEPROM.updateByte(cfgByte + 7, schedule.Thursday);
  EEPROM.updateByte(cfgByte + 8, schedule.Friday);
  EEPROM.updateByte(cfgByte + 9, schedule.Saturday);
}