#ifndef _CONFIGURATION_h
#define _CONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Configuration
{
 protected:
	int configAddress;
 	struct Data {
	 	int utcOffset;
		
		byte rly01Active;
		byte rly01OnHour;
		byte rly01OnMinute;
		byte rly01OffHour;
		byte rly01OffMinute;
		
		byte rly02Active;
		byte rly02OnHour;
		byte rly02OnMinute;
		byte rly02OffHour;
		byte rly02OffMinute;
		
		byte rly03Active;
		byte rly03OnHour;
		byte rly03OnMinute;
		byte rly03OffHour;
		byte rly03OffMinute;
		
		byte rly04Active;
		byte rly04OnHour;
		byte rly04OnMinute;
		byte rly04OffHour;
		byte rly04OffMinute;
		
		byte rly05Active;
		byte rly05OnHour;
		byte rly05OnMinute;
		byte rly05OffHour;
		byte rly05OffMinute;
		
		byte rly06Active;
		byte rly06OnHour;
		byte rly06OnMinute;
		byte rly06OffHour;
		byte rly06OffMinute;
		
		byte rly07Active;
		byte rly07OnHour;
		byte rly07OnMinute;
		byte rly07OffHour;
		byte rly07OffMinute;
		
		byte rly08Active;
		byte rly08OnHour;
		byte rly81OnMinute;
		byte rly08OffHour;
		byte rly08OffMinute;
		
		byte temp01LowAlarm;
		byte temp01HighAlarm;
		
		byte ph01LowAlarm;
		byte ph01HighAlarm;
		
		byte dp01Active;
		byte dp01OnHour;
		byte dp01OnMinute;
		byte dp01Monday;
		byte dp01Tuesday;
		byte dp01Wednesday;
		byte dp01Thursday;
		byte dp01Friday;
		byte dp01Saturday;
		byte dp01Sunday;
		byte dp01DoseAmt;
		byte dp01MlsSec;
		byte dp01Capacity;
		byte dp01Remaining;
		
		byte dp02Active;
		byte dp02OnHour;
		byte dp02OnMinute;
		byte dp02Monday;
		byte dp02Tuesday;
		byte dp02Wednesday;
		byte dp02Thursday;
		byte dp02Friday;
		byte dp02Saturday;
		byte dp02Sunday;
		byte dp02DoseAmt;
		byte dp02MlsSec;
		
		byte dp03Active;
		byte dp03OnHour;
		byte dp03OnMinute;
		byte dp03Monday;
		byte dp03Tuesday;
		byte dp03Wednesday;
		byte dp03Thursday;
		byte dp03Friday;
		byte dp03Saturday;
		byte dp03Sunday;
		byte dp03DoseAmt;
		byte dp03MlsSec;
 	};

 public:
	Configuration();
	Data data;
	
	void load();
	void save();
};

extern Configuration configuration;

#endif

