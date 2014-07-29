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
	static void wrapper_rly01_On();
	static void wrapper_rly01_Off();
	static void wrapper_rly02_On();
	static void wrapper_rly02_Off();
	static void wrapper_rly03_On();
	static void wrapper_rly03_Off();
	static void wrapper_rly04_On();
	static void wrapper_rly04_Off();
	static void wrapper_rly05_On();
	static void wrapper_rly05_Off();
	static void wrapper_rly06_On();
	static void wrapper_rly06_Off();
	static void wrapper_rly07_On();
	static void wrapper_rly07_Off();
	static void wrapper_rly08_On();
	static void wrapper_rly08_Off();
};

extern Controller controller;

#endif

