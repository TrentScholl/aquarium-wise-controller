#include <Wire.h>
#include <RTClib.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <SoftwareSerial.h>
#include <TimeAlarms.h>
#include <Time.h>
#include <EEPROMVar.h>
#include <EEPROMex.h>
#include "controller.h"

void setup()
{
	Serial.begin(9600);
	controller.setup();
}

void loop()
{
	controller.loop();
}
