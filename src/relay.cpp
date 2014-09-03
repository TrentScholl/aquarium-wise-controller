#include "relay.h"

Relay::Relay(int val, int configByte, AlarmHandler onHandler, AlarmHandler offHandler)
{
  cfgByte = configByte;
	pin = val;
  onAlarmHandler = onHandler;
  offAlarmHandler = offHandler;
	setState(HIGH);
	pinMode(pin, OUTPUT);

  schedule.active = EEPROM.readByte(cfgByte);
  schedule.onHour = EEPROM.readByte(cfgByte + 1);
  schedule.onMinute = EEPROM.readByte(cfgByte + 2);
  schedule.offHour = EEPROM.readByte(cfgByte + 3);
  schedule.offMinute = EEPROM.readByte(cfgByte + 4);
  
  updateAlarms();
}

void Relay::updateAlarms()
{
  for (int i = 0; i < 2; i = i++)
  {
    Alarm.free(alarmIds[i]);
  }
    
  if (schedule.active)
  {
    if(inSchedule())
    {
      on();
    }
    
    Alarm.alarmRepeat(schedule.onHour, schedule.onMinute, 0, onAlarmHandler);
    Alarm.alarmRepeat(schedule.offHour, schedule.offMinute, 0, offAlarmHandler);
  }
}

bool Relay::inSchedule()
{
  if (schedule.active)
  {
    tmElements_t tmSet;

    tmSet.Year = year() - 1970;
    tmSet.Month = month();
    tmSet.Day = day();
    tmSet.Hour = hour();
    tmSet.Minute = minute();
    tmSet.Second = second();

    time_t currentTime = makeTime(tmSet);
    
    tmSet.Hour = schedule.onHour;
    tmSet.Minute = schedule.onMinute;
    
    time_t powerOn = makeTime(tmSet);
    
    tmSet.Hour = schedule.offHour;
    tmSet.Minute = schedule.offMinute;
    
    time_t powerOff =  makeTime(tmSet);
    
    if (powerOff < powerOn)
    {
      if (powerOn <= currentTime)
      {
        return true;
      }
    }
    else if ((powerOn <= currentTime) && (powerOff > currentTime))
    {
      return true;
    }
  }
  
  return false;
}

Relay::Relay(int val)
{
  pin = val;
  setState(HIGH);
  pinMode(pin, OUTPUT);

  schedule.active = 0;
}

int Relay::getState()
{
	return state;
}

void Relay::on()
{
	setState(LOW);
}

void Relay::off()
{
	setState(HIGH);
}

bool Relay::isOn()
{
	if (state == LOW)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Relay::isOff()
{
	if (state == LOW)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Relay::toggle()
{
	if (state == LOW)
	{
		off();
	}
	else
	{
		on();
	}
}

void Relay::setState(byte val)
{
	if (state != val)
	{
		digitalWrite(pin, val);
		state = val;
	}
}