#include "heater.h"

Heater::Heater(Relay *rly, int configByte)
{
	cfgByte = configByte;
  
  offTemp = EEPROM.readByte(cfgByte);
  onTemp = EEPROM.readByte(cfgByte + 1);
  warnTemp = EEPROM.readByte(cfgByte + 2);
}

void Heater::saveSettings()
{
  EEPROM.updateByte(cfgByte, offTemp);
  EEPROM.updateByte(cfgByte + 1, onTemp);
  EEPROM.updateByte(cfgByte + 2, warnTemp);
}

void Heater::on()
{
  relay->on();
}

void Heater::off()
{
  relay->off();
}

bool Heater::isOn()
{
  return relay->isOn();
}

byte Heater::getOffTemp()
{
  return offTemp;
}

byte Heater::getOnTemp()
{
  return onTemp;
}

byte Heater::getWarnTemp()
{
  return warnTemp;
}

void Heater::setOffTemp(byte temp)
{
  offTemp = temp;
}

void Heater::setOnTemp(byte temp)
{
  onTemp = temp;
}

void Heater::setWarnTemp(byte temp)
{
  warnTemp = temp;
}