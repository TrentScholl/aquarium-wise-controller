#ifndef _RELAY_h
#define _RELAY_h

#include <Time.h>
#include <TimeAlarms.h>
#include <EEPROMex.h>
#include "Arduino.h"

typedef void (*AlarmHandler)();

class Relay
{
 protected:
	int state;
	int pin;
	void setState(byte val);
  int cfgByte;
  int alarmIds[2];

 public:
	Relay(int val, int configByte, AlarmHandler onHandler, AlarmHandler offHandler);
  Relay(int val);
	void on();
	void off();
	void toggle();
	int getState();
	bool isOn();
	bool isOff();
  void updateAlarms();
  bool inSchedule();

  AlarmHandler onAlarmHandler; 
  AlarmHandler offAlarmHandler; 
	  
	struct Schedule
	{
		byte active;
		byte onHour;
		byte onMinute;
		byte offHour;
		byte offMinute;
	};
  Schedule schedule;
};

#endif

