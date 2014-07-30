#include "atlas_ph.h"
#include <SoftwareSerial.h>

AtlasPH::AtlasPH(int rxPin, int txPin)
{
	SoftwareSerial mySerial(rxPin, txPin);
	atlasSerial = & mySerial;
	
	Serial.begin(38400);
	atlasSerial->begin(38400);
	
	atlasSerial->print("e\r");
	delay(50);
	atlasSerial->print("e\r");
	delay(50);
}

float AtlasPH::read()
{
	atlasSerial->print("R\r");
	
	float phVal = 0.0;
	
	while (atlasSerial->available() > 0)
	{
		phVal = Serial.parseFloat();
	}
	
	return phVal;
}