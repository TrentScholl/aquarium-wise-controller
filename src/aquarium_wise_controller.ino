#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include <EEPROMVar.h>
#include <EEPROMex.h>
#include "controller.h"

void setup()
{
	controller.setup();
}

void loop()
{
	controller.loop();
}
