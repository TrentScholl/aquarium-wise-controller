#include "EEPROMex.h"
#include "configuration.h"

Configuration::Configuration()
{
	configAddress = EEPROM.getAddress(sizeof(Data));
	load();
}

void Configuration::load()
{
	EEPROM.setMemPool(32, 4096); 
	EEPROM.readBlock(configAddress, data);
}

void Configuration::save()
{
	EEPROM.writeBlock(configAddress, data);
}

Configuration configuration;

