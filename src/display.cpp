#include "display.h"

Display::Display(int configByte)
{
	cfgByte = configByte;
  
  homeTimeout = EEPROM.readByte(cfgByte);
  dimLevel = EEPROM.readByte(cfgByte + 1);
  dimSecs = EEPROM.readByte(cfgByte + 2);
  brightness = EEPROM.readByte(cfgByte + 3);
}

void Display::saveSettings()
{
  EEPROM.updateByte(cfgByte, homeTimeout);
  EEPROM.updateByte(cfgByte + 1, dimLevel);
  EEPROM.updateByte(cfgByte + 2, dimSecs);
  EEPROM.updateByte(cfgByte + 3, brightness);
}


byte Display::getHomeTimeout()
{
  return homeTimeout;
}

byte Display::getDimLevel()
{
  return dimLevel;
}

byte Display::getDimSecs()
{
  return dimSecs;
}

byte Display::getBrightness()
{
  return brightness;
}

void Display::setHomeTimeout(byte timeout)
{
  homeTimeout = timeout;
}

void Display::setDimLevel(byte level)
{
  dimLevel = level;
}

void Display::setDimSecs(byte secs)
{
  dimSecs = secs;
}

void Display::setBrightness(byte bright)
{
  brightness = bright;
}