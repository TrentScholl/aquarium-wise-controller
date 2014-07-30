/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega 2560 or Mega ADK, Platform=avr, Package=arduino
*/

#define __AVR_ATmega2560__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\aquarium_wise_controller.ino"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\atlas_ph.cpp"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\atlas_ph.h"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\basic_switch.cpp"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\basic_switch.h"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\configuration.cpp"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\configuration.h"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\controller.cpp"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\controller.h"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\dosing_pump.cpp"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\dosing_pump.h"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\fan.cpp"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\fan.h"
#include "C:\Workspaces\Personal\aquarium_wise_controller\src\relay.h"
