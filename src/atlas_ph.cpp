#include "atlas_ph.h"
#include <SoftwareSerial.h>

AtlasPh::AtlasPh(int rxPin, int txPin)
{
	SoftwareSerial mySerial(rxPin, txPin);
	atlasSerial = & mySerial;
	
	Serial.begin(38400);
	atlasSerial->begin(38400);
	
	atlasSerial->print("E\r");
	delay(50);
	atlasSerial->print("E\r");
	delay(50);
}

float AtlasPh::getPh(int tempVal)
{
	atlasSerial->print("R\r");
	
	float phVal = 0.0;
	
	while (atlasSerial->available() > 0)
	{
		phVal = Serial.parseFloat();
	}
	
	return phVal;
}

void AtlasPh::setHighAlarmPh(char phVal)
{
	if (phVal > 8)
	{
		phVal = 8;
	}
	else if (phVal < 4)
	{
		phVal = 4;
	}

	highAlarmPh = (uint8_t)phVal;
}

void AtlasPh::setLowAlarmPh(char phVal)
{
	if (phVal > 8)
	{
		phVal = 8;
	}
	else if (phVal < 4)
	{
		phVal = 4;
	}

	lowAlarmPh = (uint8_t)phVal;
}