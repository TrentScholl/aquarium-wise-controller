#ifndef _CONTROLLER_h
#define _CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "relay.h"

class Controller
{
 private:
	
 public:
	void setup();
	void loop();
	void rly_State(Relay rly, byte state);
	static void rly01_On();
	static void rly01_Off();
	static void rly02_On();
	static void rly02_Off();
	static void rly03_On();
	static void rly03_Off();
	static void rly04_On();
	static void rly04_Off();
	static void rly05_On();
	static void rly05_Off();
	static void rly06_On();
	static void rly06_Off();
	static void rly07_On();
	static void rly07_Off();
	static void rly08_On();
	static void rly08_Off();
	void check_Temperatures();
};

extern Controller controller;

#endif

