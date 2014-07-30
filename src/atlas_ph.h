#ifndef _ATLAS_PH_h
#define _ATLAS_PH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SoftwareSerial.h>

class AtlasPH
{
 protected:
	SoftwareSerial *atlasSerial;
 public:
	AtlasPH(int rxPin, int txPin);
	float read();
};

#endif

