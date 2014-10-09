#include <Wire.h>
#include <EEPROMex.h>
#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Geometry.h>
#include <uText.h>
#include <RTClib.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <avr/wdt.h>

#include "debug.h"
#include "pins.h"
#include "dosing_pump.h"
#include "relay.h"
#include "atlas_ph.h"
#include "buzzer.h"
#include "display.h"
#include "heater.h"
#include "fonts.h"
#include "theme.h"
//#include <SPI.h>
//#include <UIPEthernet.h>

//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//EthernetClient client;

const byte numRelays = 8;
Relay relays[numRelays] = {
  Relay(relayLights1Pin, 1, alarm_rlyLight1_on, alarm_rlyLight1_off),
  Relay(relayLights2Pin, 8, alarm_rlyLight2_on, alarm_rlyLight2_off),
  Relay(relayFilter1Pin),
  Relay(relayFilter2Pin),
  Relay(relayHeater1Pin),
  Relay(relayHeater2Pin),
  Relay(relayCirculationPin, 43, alarm_rlyCirc_on, alarm_rlyCirc_off),
  Relay(relayCO2Pin, 50, alarm_rlyCO2_on, alarm_rlyCO2_off)
};

const byte numDosingPumps = 3;
DosingPump dosingPumps[numDosingPumps] = {
  DosingPump(dosingPumpMacroPin, 58, alarm_macro_dose),
  DosingPump(dosingPumpMicroPin, 75, alarm_micro_dose),
  DosingPump(dosingPumpGlutPin, 92, alarm_glut_dose)
};

Heater heater01(&relays[4], 138);
Heater heater02(&relays[5], 138);

AtlasPh pH01(&Serial1);

Display display01(144);

byte backLight = 255;

boolean backlightTouch = true;

RTC_DS1307 RTC;

OneWire oneWire(oneWirePin);

DallasTemperature dallasTemperatureSensors(&oneWire);

DeviceAddress temperatureProbe01;
DeviceAddress temperatureProbe02;

UTFT myGLCD(SSD1289, tftRsPin, tftWrPin, tftCsPin, tftRstPin);
UTouch myTouch(touchClkPin, touchCsPin, touchDinPin, touchDoutPin, touchIrqPin);
UTFT_Geometry geo(&myGLCD);
uText utext(&myGLCD, 240, 320);

byte dispScreen = 0;
byte selectedItem = 0;

char prevtempString[7];
char prevphString[5];

struct RTC_T
{  
  byte tHour;
  byte tMinute;
  byte tSecond;
  byte tDow;
  byte tDay;
  byte tMonth;
  byte tYear;
} prevRTC, saveRTC;

unsigned long prevMillisTouch = 0;
unsigned long prevMillis5sec = 0;
unsigned long millisDim = 0;
unsigned long millisHome = 0;

int freeRam ()
{
  // Returns available SRAM
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup()
{  
  Serial.begin(9600);
  
  DEBUG_PRINTLN("Starting Aquarium Wise");
  firstRunSetup();

  myGLCD.InitLCD(PORTRAIT);
  
  myGLCD.fillScr(THEME_BACK.r, THEME_BACK.g, THEME_BACK.b);
  
  myTouch.InitTouch(PORTRAIT);
  myTouch.setPrecision(PREC_MEDIUM);

  Wire.begin();
  RTC.begin();
  dallasTemperatureSensors.begin();
  
  setSyncProvider(syncProvider);

  pinMode(screenBrightnessPin, OUTPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(alarmPin, OUTPUT);

  setup_TemperatureSensors();

  analogWrite(screenBrightnessPin, display01.getBrightness());

  millisDim = millis();
  
  setup_Relays();
  
  update_alarms();
  
  buzzer.init();
  buzzer.beep(2, 50);
  
  // Ethernet.begin(mac);

  updateTimeDate();
  screenHome();
}

unsigned long lastTempRequest = 0;
int  delayInMillis = 0;

void setup_TemperatureSensors()
{
  DEBUG_PRINTLN("Setting up temperature sensors");
  dallasTemperatureSensors.setResolution(10);
  dallasTemperatureSensors.setWaitForConversion(false);

  dallasTemperatureSensors.getAddress(temperatureProbe01, 0);
  dallasTemperatureSensors.getAddress(temperatureProbe02, 1);
  
  dallasTemperatureSensors.setHighAlarmTemp(temperatureProbe01, heater01.getOffTemp());
  dallasTemperatureSensors.setLowAlarmTemp(temperatureProbe01, heater01.getWarnTemp());
  
  dallasTemperatureSensors.setAlarmHandler(&alarm_temperature);
  
  dallasTemperatureSensors.requestTemperatures();
  delayInMillis = 750 / (1 << (12 - 10));
  lastTempRequest = millis();
}

void setup_Relays()
{
  DEBUG_PRINTLN("Setting up relays");
  
  relays[2].on();
  relays[3].on();
  relays[4].on();
  relays[5].on();
}

time_t syncProvider()
{
  return RTC.now().unixtime();
}

void loop()
{  
  unsigned long currentMillis = millis();

  if (myTouch.dataAvailable())
  {
    if (currentMillis - prevMillisTouch > 250)
    {
      if (backlightTouch == false)
      {
        backLight = 255;
        analogWrite(screenBrightnessPin, backLight);
        backlightTouch = true;
      }
      
      prevMillisTouch = currentMillis;
      millisDim = currentMillis;
      millisHome = currentMillis;
      
      processMyTouch();
    }
  }

  if (currentMillis - prevMillis5sec > 1000)
  {
    prevMillis5sec = millis();
    updateTimeDate();

    check_Ph(dispScreen == 1);
    check_Temperatures(dispScreen == 1);
  }

  if (display01.getDimSecs() != 0)
  {
    if (display01.getDimLevel() != 0)
    {
      if (backlightTouch == true)
      {
        unsigned long pastMillis = (currentMillis - millisDim);
        if (pastMillis > (1000 * display01.getDimSecs()))
        {
          backlightTouch=false;
        }
      }
      else
      {
        autoBrightness();
      }
    }
  }
  
  if (display01.getHomeTimeout() != 0)
  {
    if ((dispScreen != 1) && (dispScreen != 2))
    {
      unsigned long pastMillis = (currentMillis - millisHome);
      if (pastMillis > (60000 * display01.getHomeTimeout()))
      {
        if (dispScreen == 10)
        {
          drawPleaseWait();
          for (byte i = 0; i < numDosingPumps; i++)
          {
            dosingPumps[i].saveSettings();
          }
        }
        screenHome();
      }
    }
  }

  Alarm.delay(0);
}

void drawSmallRelayStatus(int relay, int state, int x, int y)
{
  if (state == 1)
  {
    myGLCD.setColor(35, 175, 68);
    myGLCD.fillRect(x, y, x+26, y+25);
    
    utext.setForeground(255, 255, 255);
    utext.setBackground(35, 175, 68);
  }
  else
  {
    myGLCD.setColor(222, 229, 231);
    myGLCD.drawLine(x, y, x+26, y);
    myGLCD.drawLine(x, y+25, x+26, y+25);
    myGLCD.drawLine(x+26, y, x+26, y+25);
    myGLCD.drawLine(x, y, x, y+25);
    
    myGLCD.setColor(255, 255, 255);
    myGLCD.fillRect(x+1, y+1, x+24, y+23);
     
    utext.setForeground(88, 102, 110);
    utext.setBackground(255, 255, 255);
  }
  
  utext.setFont(FontAwesomeRegular14);
  
  switch (relay)
  {
    case 0:
      utext.print(x+8, y+3, "2");
      break;
    case 1:
      utext.print(x+8, y+3, "2");
      break;
    case 2:
      utext.print(x+6, y+3, "3");
      break;
    case 3:
      utext.print(x+6, y+3, "3");
      break;
    case 4:
      utext.print(x+8, y+4, "4");
      break;
    case 5:
      utext.print(x+8, y+4, "4");
      break;
    case 6:
      utext.print(x+5, y+2, "5");
      break;
    case 7:
      utext.print(x+3, y+3, "6");
      break;
  }
}

void drawHeader(char* icon, char* title)
{
  myGLCD.setColor(THEME_HEAD_BACK.r, THEME_HEAD_BACK.g, THEME_HEAD_BACK.b);
  myGLCD.fillRect(0, 0, 239, 39);

  utext.setFont(FontAwesomeRegular14);
  utext.setForeground(THEME_HEAD_FORE.r, THEME_HEAD_FORE.g, THEME_HEAD_FORE.b);
  utext.setBackground(THEME_HEAD_BACK.r, THEME_HEAD_BACK.g, THEME_HEAD_BACK.b);
  
  // Lines under header
  myGLCD.setColor(222, 224, 224);
  myGLCD.drawLine(0, 40, 239, 40); 
  
  myGLCD.setColor(237, 239, 239);
  myGLCD.drawLine(0, 41, 239, 41);
  
  // Sub header
  myGLCD.setColor(246, 248, 248);
  myGLCD.fillRect(0, 42, 239, 59);
  
  // Line under sub header
  myGLCD.setColor(222, 239, 231);
  myGLCD.drawLine(0, 60, 239, 60);
    
  utext.setForeground(THEME_SUBHEAD_FORE.r, THEME_SUBHEAD_FORE.g, THEME_SUBHEAD_FORE.b);
  utext.setBackground(THEME_SUBHEAD_BACK.r, THEME_SUBHEAD_BACK.g, THEME_SUBHEAD_BACK.b);
  
  utext.setFont(Arial11);
  utext.print(27, 44, title);
  
  utext.setFont(FontAwesomeRegular14);
  utext.print(4, 43, icon);
  
  utext.setForeground(THEME_HEAD_FORE.r, THEME_HEAD_FORE.g, THEME_HEAD_FORE.b);
  utext.setBackground(THEME_HEAD_BACK.r, THEME_HEAD_BACK.g, THEME_HEAD_BACK.b);
  
  switch (dispScreen)
  {
    case 0:
    case 1:
      utext.print(32, 12, "8");
      utext.print(110, 12, "9");
      utext.print(188, 12, "a");
      break;
    case 2:
    case 3:
    case 4:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 16:
        utext.print(160, 12, "a");
        utext.print(60, 12, "1");
      break;
  }

  prevRTC.tMinute = 0;
  prevRTC.tHour = 0;

  updateTimeDate();
}

void drawLargeButton(char icon, char* text, int state, int x, int y)
{
  if (state == 1)
  {
    myGLCD.setColor(35, 175, 68);
    myGLCD.fillRect(x, y, x + 34, y + 33);
    
    myGLCD.setColor(39, 194, 76);
    myGLCD.fillRect(x+34, y, x + 113, y + 33);
    
    utext.setForeground(255, 255, 255);
    utext.setBackground(35, 175, 68);
  } 
  else
  {
    myGLCD.setColor(222, 229, 231);
    myGLCD.fillRect(x, y, x + 113, y + 33);
    
    myGLCD.setColor(255, 255, 255);
    myGLCD.fillRect(x + 1, y + 1, x + 112, y + 32);
    
    myGLCD.setColor(222, 229, 231);
    myGLCD.drawLine(x + 34, y, x + 34, y + 33);
    
    utext.setForeground(88, 102, 110);
    utext.setBackground(255, 255, 255);
  }
  
  utext.setFont(Arial11);
  utext.print(x+46, y+10, text);
  
  utext.setFont(FontAwesomeRegular14);
  
  switch (icon)
  {
    case '2':
      utext.print(x+12, y+8, "2");
      break;
    case '3':
      utext.print(x+10, y+8, "3");
      break;
    case '4':
      utext.print(x+12, y+8, "4");
      break;
    case '5':
      utext.print(x+10, y+8, "5");
      break;
    case '6':
      utext.print(x+8, y+8, "6");
      break;
    case '7':
      utext.print(x+13, y+7, "7");
      break;
    case 'c':
      utext.print(x+12, y+8, "c");
      break;
    case '8':
      utext.print(x+12, y+8, "8");
      break;
    case '9':
      utext.print(x+9, y+8, "9");
      break;
    case 'e':
      utext.print(x+12, y+8, "e");
      break;
    case 'b':
      utext.print(x+12, y+8, "b");
      break;
    case 'd':
      utext.print(x+12, y+8, "d");
      break;
  }
}

void drawBackground()
{
    //myGLCD.fillScr(THEME_BACK.r, THEME_BACK.g, THEME_BACK.b);
    myGLCD.setColor(THEME_SUBHEAD_BACK.r, THEME_SUBHEAD_BACK.g, THEME_SUBHEAD_BACK.b);
    myGLCD.fillRect(0, 61, 239, 319);
}

void drawSpinner(int x, int y, String val)
{
  myGLCD.setColor(222, 229, 231);
  myGLCD.fillRect(x, y, x + 151, y + 33);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(x + 1, y + 1, x + 150, y + 32);
  
  myGLCD.setColor(222, 229, 231);
  myGLCD.drawLine(x + 31, y, x + 31, y + 31);
  myGLCD.drawLine(x + 120, y, x + 120, y + 31);

  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(x + 10, y + 16, x + 20, y + 17);

  myGLCD.fillRect(x + 130, y + 16, x + 141, y + 17);
  myGLCD.fillRect(x + 135, y + 11, x + 136, y + 22);
  
  utext.setFont(Arial11);
  utext.setForeground(88, 102, 110);
  utext.print((x + 32) + findCenterText(val, x + 32, x + 119), y + 10, val);
}

void drawLargeBlueButton(int x, int y, char* text)
{
  myGLCD.setColor(31, 164, 205);
  myGLCD.fillRect(x, y, x+33, y+33);
  
  myGLCD.setColor(35, 183, 229);
  myGLCD.fillRect(x+33, y, x+110, y+33);
  
  utext.setForeground(255, 255, 255);
  utext.setBackground(35, 183, 229);
  utext.setFont(Arial11);
  utext.print(x + 55, y + 10, text);
}

void screenHome()
{
  DEBUG_PRINTLN("Drawing home screen");
  if (dispScreen != 1)
  {
    dispScreen = 1;
    drawHeader("1", "Home");
    drawBackground();
    
    memset(prevtempString, 0, sizeof prevtempString);
    memset(prevphString, 0, sizeof prevtempString);
  } 
  
  dispScreen = 1;
  
  // Row 1
  myGLCD.setColor(THEME_PRIMARY_BACK.r, THEME_PRIMARY_BACK.g, THEME_PRIMARY_BACK.b);

  // Column 1
  myGLCD.fillRect(4, 64, 117, 121);
  
  // Column 2
  myGLCD.fillRect(122, 64, 235, 121);
  
  // End Row 1
  
  // Row 2
  drawSmallRelayStatus(0, relays[0].isOn(), 4, 125);
  drawSmallRelayStatus(1, relays[1].isOn(), 33, 125);
  drawSmallRelayStatus(2, relays[2].isOn(), 62, 125);
  drawSmallRelayStatus(3, relays[3].isOn(), 91, 125);
  drawSmallRelayStatus(4, relays[4].isOn(), 122, 125);
  drawSmallRelayStatus(5, relays[5].isOn(), 151, 125);
  drawSmallRelayStatus(6, relays[6].isOn(), 180, 125);
  drawSmallRelayStatus(7, relays[7].isOn(), 209, 125);
  
  // End Row 2
  
  // Row 3
  myGLCD.setColor(THEME_PRIMARY_BACK.r, THEME_PRIMARY_BACK.g, THEME_PRIMARY_BACK.b);
  
  // Column 1
  myGLCD.fillRect(4, 154, 79, 241);
  
  // Column 2
  myGLCD.fillRect(83, 154, 156, 241);
  
  // Column 3
  myGLCD.fillRect(160, 154, 235, 241);
  
  // End Row 3
  
  // Row 4
  
  // Column 1
  
  myGLCD.fillRect(4, 245, 117, 315);
  
  // Column 2
  
  myGLCD.fillRect(122, 245, 235, 315);
  
 // End Row 4
 
  drawDoseChart(42, 202, 0, "Macro", THEME_MACRO);
  drawDoseChart(119, 202, 1, "Micro", THEME_MICRO);
  drawDoseChart(198, 202, 2, "Glut", THEME_GLUT);
  
  utext.setFont(Arial7);
  utext.setForeground(88, 102, 110);
  utext.print(48, 108, "Temp");
  utext.print(172, 108, "pH");
  
  utext.getTextWidth("Temp");
 
  check_Temperatures(true);
  check_Ph(true);
}

int findCenterText(String text, int x1, int x2)
{
  int textWidth = utext.getTextWidth(text);
    
  return ((x2 - x1) - textWidth) / 2;
}

void drawPleaseWait()
{
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(30, 120, 209, 180);
    
    myGLCD.setColor(THEME_SUBHEAD_BACK.r, THEME_SUBHEAD_BACK.g, THEME_SUBHEAD_BACK.b);
    myGLCD.fillRect(31, 121, 208, 179);
    
    
    
    utext.setFont(Arial11);
    utext.setForeground(0, 0, 0);
    utext.print(80, 145, "Please wait...");
}

void drawDoseChart(int x, int y, byte pump, char* label, Color arcColor)
{
  myGLCD.setColor(232, 239, 240);
  myGLCD.fillCircle(x, y, 31);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillCircle(x, y, 28);
  
  int arcAngle = (360) * (((float)dosingPumps[pump].getRemainingVol() / dosingPumps[pump].getVol()));
  
  myGLCD.setColor(arcColor.r, arcColor.g, arcColor.b);
  geo.drawArc(x, y, 30, 0, arcAngle, 3);
  
  String remaining = String(dosingPumps[pump].getRemainingVol() / dosingPumps[pump].getDoseAmt());
  
  utext.setForeground(88, 102, 110);
  
  utext.setFont(Arial11);
  utext.print((x - 30) + findCenterText(remaining, x - 30, x + 30), y - 7, remaining);
  
  utext.setFont(Arial7);
  utext.print((x - 30) + findCenterText(label, x - 30, x + 30), y - 44, label);
}

void screenFeeding()
{
  DEBUG_PRINTLN("Drawing feeding screen");
  dispScreen = 2;
  
  drawHeader("8", "Feeding");
  drawBackground();
}

void screenPower()
{
  DEBUG_PRINTLN("Drawing power screen");
  if (dispScreen != 3)
  {
    dispScreen = 3;
    drawHeader("7", "Power");
    drawBackground();
  }
  
  drawLargeButton('2', "Lights 1", relays[0].isOn(), 4, 64);
  drawLargeButton('2', "Lights 2", relays[1].isOn(), 122, 64);
  
  drawLargeButton('3', "Filter 1", relays[2].isOn(), 4, 102);
  drawLargeButton('4', "Filter 2", relays[3].isOn(), 122, 102);
  
  drawLargeButton('5', "Heater 1", relays[4].isOn(), 4, 140);
  drawLargeButton('6', "Heater 2", relays[5].isOn(), 122, 140);
  
  drawLargeButton('7', "Circulat...", relays[6].isOn(), 4, 178);
  drawLargeButton('7', "CO2", relays[7].isOn(), 122, 178);
  
  drawLargeButton('9', "All On", 1, 4, 283);
  drawLargeButton('9', "All Off", 0, 122, 283);
  
  updateTimeDate();
}

void screenSettings()
{
  DEBUG_PRINTLN("Drawing settings screen");
  dispScreen = 4;

  drawHeader("1", "Home");
  drawBackground();
  
  drawLargeButton('2', "Lights", 0, 4, 64);
  drawLargeButton('c', "Schedule", 0, 122, 64);
  
  drawLargeButton('4', "Heater", 0, 4, 102);
  drawLargeButton('8', "Feeding", 0, 122, 102);
  
  drawLargeButton('e', "Dosing", 0, 4, 140);
  drawLargeButton('b', "Clock", 0, 122, 140);
  
  drawLargeButton('d', "Screen", 0, 4, 178);
}

void screenLights()
{
  DEBUG_PRINTLN("Drawing light settings screen");
  
  dispScreen = 5; 
  
  drawHeader("2", "Lights");
  drawBackground();
}

void screenClock()
{
  DEBUG_PRINTLN("Drawing clock settings screen");
  
  dispScreen = 6; 
  
  drawHeader("b", "Clock");
  drawBackground();
}

void screenFeedSettings()
{
  DEBUG_PRINTLN("Drawing feed settings screen");
  
  dispScreen = 7; 
  
  drawHeader("8", "Feeding");
  drawBackground();
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(4, 64, 235, 105);

  utext.print(12, 81, "Mins.");
    
  drawSpinner(80, 68, String(EEPROM.readByte(150)));
  
  drawLargeButton('2', "Lights 1", 0, 4, 110);
  drawLargeButton('2', "Lights 2", 0, 122, 110);
    
  drawLargeButton('3', "Filter", 0, 4, 148);
  drawLargeButton('4', "Heater", 0, 122, 148);
    
  drawLargeButton('5', "Circulat..", 0, 4, 186);
  drawLargeButton('6', "CO2", 0, 122, 186);
    
  drawLargeButton('7', "AUX1", 0, 4, 224);
  drawLargeButton('7', "AUX2", 0, 122, 224);
}

void screenHeater()
{
  DEBUG_PRINTLN("Drawing heater settings screen");

  dispScreen = 8; 
  
  drawHeader("4", "Heater");
  drawBackground();
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(4, 64, 235, 190);

  utext.print(12, 81, "Off");
  utext.print(12, 124, "On");
  utext.print(12, 163, "Warn.");
  
  drawSpinner(80, 68, String(heater01.getOffTemp()));
  drawSpinner(80, 109, String(heater01.getOnTemp()));
  drawSpinner(80, 150, String(heater01.getWarnTemp()));
}

void screenSchedule()
{
  DEBUG_PRINTLN("Drawing schedule settings screen");
  
  dispScreen = 9; 
  
  drawHeader("c", "Schedule");
  drawBackground();
  
  drawLargeButton('7', "Power", 0, 4, 64);
  drawLargeButton('e', "Dosing", 0, 122, 64);
}

void drawDosingPumpSettings(byte pump)
{  
  drawSpinner(80, 105, String(dosingPumps[pump].getDoseAmt()));
  drawSpinner(80, 146, String(dosingPumps[pump].getMlSec()));
  drawSpinner(80, 187, String(dosingPumps[pump].getVol()));

  //utext.setForeground(88, 102, 110);
  //utext.setBackground(237, 241, 242);
  //utext.setFont(Arial11);
}

void drawDosingPumpTabs(byte selected)
{
  myGLCD.setColor(222, 229, 231);
  myGLCD.fillRect(4, 64, 235, 97);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(5, 65, 234, 96);
  
  myGLCD.setColor(222, 229, 231);
  myGLCD.drawLine(81, 65, 81, 96);
  myGLCD.drawLine(159, 65, 159, 96);
  
  myGLCD.setColor(237, 241, 242);
  
  switch (selected)
  {
    case 0:
      myGLCD.fillRect(5, 65, 80, 96);  
      break;
    case 1:
      myGLCD.fillRect(82, 65, 158, 96 );  
      break;
    case 2:
      myGLCD.fillRect(160, 65, 234, 96);  
      break;
  } 

  utext.setForeground(88, 102, 110);
  utext.setBackground(237, 241, 242);
  utext.setFont(Arial11);
  utext.print(18, 74, "Macros");
  utext.print(98, 74, "Micros");
  utext.print(184, 74, "Glut");
}

void screenDosing(byte pump)
{
  DEBUG_PRINTLN("Drawing dosing settings screen");
  
  dispScreen = 10;
  selectedItem = pump;

  drawHeader("e", "Dosing");
  drawBackground();

  drawDosingPumpTabs(selectedItem);

  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(4, 101, 235, 316);

  utext.print(12, 118, "Dose");
  utext.print(12, 161, "Rate");
  utext.print(12, 200, "Volume");
  
  drawDosingPumpSettings(selectedItem);
  
  drawLargeBlueButton(8, 280, "Fill");
  drawLargeBlueButton(121, 280, "Test");
}

char* friendlyTime(byte onHour, byte onMinute)
{
  static char str[5];

  sprintf(str, "%02d:%02d", onHour, onMinute);
  
  return str;
}

void screenPwrSchedule()
{
  DEBUG_PRINTLN("Drawing power schedule settings screen");
  
  dispScreen = 11; 
  
  drawHeader("c", "Power Schedule");
  drawBackground();
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(4, 64, 235, 230);
    
  utext.setForeground(88, 102, 106);
  utext.setFont(Arial11);
  utext.print(120, 66, "On");
  utext.print(170, 66, "Off");
    
  myGLCD.setColor(222, 229, 231);
  myGLCD.drawLine(4, 83, 235, 83);
    
  drawPowerScheduleLineItem("Lights 1", relays[0], 6, 91);
  drawPowerScheduleLineItem("Lights 2", relays[1], 6, 118);
  drawPowerScheduleLineItem("Circulation", relays[6], 6, 145);
  drawPowerScheduleLineItem("CO2", relays[7], 6, 172);
}

void screenPwrScheduleItem(int itemNo)
{
  dispScreen = 12;  
  
  drawHeader("c", "Power Schedule");
  drawBackground();
}

void screenResyncLights()
{
  drawHeader("2", "Lights");
  drawBackground();
}

void screenLightRamps()
{
  dispScreen = 13; 
  
  drawHeader("2", "Lights");
  drawBackground();
}

void screenLightRampItem(byte rampNo)
{
  dispScreen = 14; 
  
  drawHeader("2", "Lights");
  drawBackground();
}

void screenDosingSched()
{
  dispScreen = 15;
  
  drawHeader("c", "Dosing Schedule");
  drawBackground();
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(4, 64, 235, 230);
  
  utext.setForeground(88, 102, 106);
  utext.setFont(Arial11);
  utext.print(80, 66, "On");
  utext.print(130, 66, "S M T W T F S");
  
  myGLCD.setColor(222, 229, 231);
  myGLCD.drawLine(4, 83, 235, 83);
  
  drawDoingPumpScheduleLineItem("Macro", dosingPumps[0], 6, 91);
  drawDoingPumpScheduleLineItem("Micro", dosingPumps[1], 6, 118);
  drawDoingPumpScheduleLineItem("Glut", dosingPumps[2], 6, 145);
}

void drawPowerScheduleLineItem(String name, Relay rly, byte x, byte y)
{
  utext.print(x, y, name);
  utext.print(x + 104, y, friendlyTime(rly.schedule.onHour, rly.schedule.onMinute));
  utext.print(x + 154, y, friendlyTime(rly.schedule.offHour, rly.schedule.offMinute));

  if (rly.schedule.active)
  {
    utext.print(x + 209, y, "Y");
  }
  else
  {
    utext.print(x + 209, y, "N");  
  }
  
  myGLCD.setColor(222, 229, 231);
  myGLCD.drawLine(4, y + 21, 235, y + 21);
}

void drawDoingPumpScheduleLineItem(String name, DosingPump dp, byte x, byte y)
{
  utext.print(x, y, name);
  utext.print(x + 64, y, friendlyTime(dp.schedule.onHour, dp.schedule.onMinute));

  myGLCD.setColor(17, 153, 196);
  myGLCD.setBackColor(17, 153, 196);
  
  if (dp.schedule.Sunday)
  {
    myGLCD.fillRect(x + 125, y + 2, x + 135, y + 13);
  }
  else
  {
    myGLCD.drawRect(x + 125, y + 2, x + 135, y + 13);
  }
  
  if (dp.schedule.Monday)
  {
    myGLCD.fillRect(x + 139, y + 2, x + 149, y + 13);
  }
  else
  {
    myGLCD.drawRect(x + 139, y + 2, x + 149, y + 13);
  }
  
  if (dp.schedule.Tuesday)
  {
    myGLCD.fillRect(x + 153, y + 2, x + 163, y + 13);
  }
  else
  {
    myGLCD.drawRect(x + 153, y + 2, x + 163, y + 13);
  }
  
  if (dp.schedule.Wednesday)
  {
    myGLCD.fillRect(x + 167, y + 2, x + 177, y + 13);
  }
  else
  {
    myGLCD.drawRect(x + 167, y + 2, x + 177, y + 13);
  }
  
  if (dp.schedule.Thursday)
  {
    myGLCD.fillRect(x + 181, y + 2, x + 191, y + 13);
  }
  else
  {
    myGLCD.drawRect(x + 181, y + 2, x + 191, y + 13);
  }
  
  if (dp.schedule.Friday)
  {
    myGLCD.fillRect(x + 195, y + 2, x + 205, y + 13);
  }
  else
  {
    myGLCD.drawRect(x + 195, y + 2, x + 205, y + 13);
  }
  
  if (dp.schedule.Saturday)
  {
    myGLCD.fillRect(x + 209, y + 2, x + 219, y + 13);
  }
  else
  {
    myGLCD.drawRect(x + 209, y + 2, x + 219, y + 13);
  }
  
  myGLCD.setColor(222, 229, 231);
  myGLCD.drawLine(4, y + 21, 235, y + 21);
}
  
void screenDosingSchedItem(byte pumpNo)
{
  dispScreen = 15; 
  
  drawHeader("c", "Dosing Schedule");
  drawBackground();
}

void screenScreen()
{
  DEBUG_PRINTLN("Drawing screen settings screen");
  
  dispScreen = 16; 
  
  drawHeader("d", "Screen");
  drawBackground();
    
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(4, 64, 235, 230);

  utext.print(12, 81, "Ret. Time");
  utext.print(12, 124, "Dim Lev.");
  utext.print(12, 163, "Dim Time");
  utext.print(12, 202, "Bright");
    
  drawSpinner(80, 68, String(display01.getHomeTimeout()));
  drawSpinner(80, 109, String(display01.getDimLevel()));
  drawSpinner(80, 150, String(display01.getDimSecs()));
  drawSpinner(80, 191, String(display01.getBrightness()));
}

boolean inBounds(int touchPointx, int touchPointy, int point1x, int point1y, int point2x, int point2y)
{
  if ((touchPointx >= point1x) && (touchPointx <= point2x) && (touchPointy >= point1y) && (touchPointy <= point2y))
  {
    return true;
  }
  
  return false;
}

void processMyTouch()
{
  DEBUG_PRINTLN("Processing touch event");
  
  myTouch.read();

  int x = myTouch.getX();
  int y = myTouch.getY();

  switch (dispScreen) 
  {              
    case 1:
      if (inBounds(x, y, 0, 0, 79, 39))
      {
        screenFeeding();
      }
      else if (inBounds(x, y, 80, 0, 165, 39))
      {
        screenPower();
      }
      else if (inBounds(x, y, 166, 0, 239, 36))
      {
        screenSettings();
      }
      break;
    
    case 2:
      if (inBounds(x, y, 80, 0, 165, 39))
      {     
        screenHome();
      }
      break;
    
    case 3:
      if (inBounds(x, y, 80, 0, 165, 39))
      {     
        screenHome();
      }
      else if (inBounds(x, y, 4, 283, 117, 316))
      {
        for (byte i = 0; i <= numRelays; i++)
        {
          relays[i].on();
        }
        
        screenPower();
      }
      else if (inBounds(x, y, 122, 283, 235, 316))
      {
        for (byte i = 0; i <= numRelays; i++)
        {
          relays[i].off();
        }

        screenPower();
      }
      else if (inBounds(x, y, 4, 64, 117, 97))
      {
        if (relays[0].isOn() == 0)
        {
          relays[0].on();
        }
        else if (relays[0].isOn() == 1)
        {
          relays[0].off();
        }

        drawLargeButton('2', "Lights 1", relays[0].isOn(), 4, 64);
      }
      else if (inBounds(x, y, 122, 64, 235, 97))
      {
        if (relays[1].isOn() == 0)
        {
          relays[1].on();
        }
        else if (relays[1].isOn() == 1)
        {
          relays[1].off();
        }
        
        drawLargeButton('2', "Lights 2", relays[1].isOn(), 122, 64);
      }
      else if (inBounds(x, y, 4, 102, 117, 135))
      {
        if (relays[2].isOn() == 0)
        { 
          relays[2].on();
        }
        else if (relays[2].isOn() == 1)
        {
          relays[2].off();
        }

        drawLargeButton('3', "Filter 1", relays[2].isOn(), 4, 102);
      }
      else if (inBounds(x, y, 122, 102, 235, 135))
      {
        if (relays[3].isOn() == 0)
        {
          relays[3].on();
        }
        else if (relays[3].isOn() == 1)
        {
          relays[3].off();
        }

        drawLargeButton('4', "Filter 2", relays[3].isOn(), 122, 102);
      }
      else if (inBounds(x, y, 4, 140, 117, 173))
      {
        if (relays[4].isOn() == 0)
        {
          relays[4].on();
        }
        else if (relays[4].isOn() == 1)
        {
          relays[4].off();
        }

        drawLargeButton('5', "Heater 1", relays[4].isOn(), 4, 140);
      }
      else if (inBounds(x, y, 122, 130, 235, 173))
      {
        if (relays[5].isOn() == 0)
        {
          relays[5].on();
        }
        else if (relays[5].isOn() == 1)
        {
          relays[5].off();
        }

        drawLargeButton('6', "Heater 2", relays[5].isOn(), 122, 140);
      }
      else if (inBounds(x, y, 4, 178, 117, 211))
      {
        if (relays[6].isOn() == 0)
        {
          relays[6].on();
        }
        else if (relays[6].isOn() == 1)
        {
          relays[6].off();
        }
        
        drawLargeButton('7', "Circulat.", relays[6].isOn(), 4, 178);
      }
      else if (inBounds(x, y, 122, 178, 235, 211))
      {
        if (relays[7].isOn() == 0)
        {
          relays[7].on();
        }
        else if (relays[7].isOn() == 1)
        {
          relays[7].off();
        }

        drawLargeButton('7', "CO2", relays[7].isOn(), 122, 178);
      }
      break;
     
    case 4:
      if (inBounds(x, y, 80, 0, 165, 39))
      {     
        screenHome();
      }
      else if (inBounds(x, y, 4, 64, 117, 97))
      {
        screenLights();
      }
      else if (inBounds(x, y, 122, 139, 235, 172))
      {
        screenClock();
      }
      else if (inBounds(x, y, 122, 102, 235, 135))
      {
        screenFeedSettings();
      }
      else if (inBounds(x, y, 122, 64, 235, 97))
      {
        screenSchedule();
      }
      else if (inBounds(x, y, 4, 102, 117, 135))
      {
        screenHeater();
      }
      else if (inBounds(x, y, 4, 139, 117, 172))
      {
        screenDosing(0);
      }
      else if (inBounds(x, y, 4, 177, 117, 210))
      {
        screenScreen();
      }
    
      break;

    case 5:
      if (inBounds(x, y, 0, 0, 119, 39))
      {
        screenHome();
      }
      else if (inBounds(x, y, 120, 0, 239, 39))
      {
        screenSettings();
      }
      break;

    case 6:
      if (inBounds(x, y, 0, 0, 119, 39))
      {
        screenHome();
      }
      else if (inBounds(x, y, 120, 0, 239, 39))
      {
        screenSettings();
      }
      break;

    case 7:
      if (inBounds(x, y, 0, 0, 119, 39))
      {
        screenHome();
      }
      else if (inBounds(x, y, 120, 0, 239, 39))
      {
        screenSettings();
      }
      break;

    case 8:
      if (inBounds(x, y, 0, 0, 119, 39))
      {
        drawPleaseWait();
        
        heater01.saveSettings();
        
        dallasTemperatureSensors.setHighAlarmTemp(temperatureProbe01, heater01.getOffTemp());
        dallasTemperatureSensors.setLowAlarmTemp(temperatureProbe01, heater01.getWarnTemp());

        screenHome();
      }
      else if (inBounds(x, y, 120, 0, 239, 39))
      {
        drawPleaseWait();
        
        heater01.saveSettings();
        
        dallasTemperatureSensors.setHighAlarmTemp(temperatureProbe01, heater01.getOffTemp());
        dallasTemperatureSensors.setLowAlarmTemp(temperatureProbe01, heater01.getWarnTemp());
        
        screenSettings();
      }
      else if (inBounds(x, y, 80, 68, 111, 101))
      {
        heater01.setOffTemp(heater01.getOffTemp() - 1);
        screenHeater();
      }
      else if (inBounds(x, y, 200, 68, 231, 101))
      {
        heater01.setOffTemp(heater01.getOffTemp() + 1);
        screenHeater();
      }
      else if (inBounds(x, y, 80, 109, 111, 142))
      {
        heater01.setOnTemp(heater01.getOnTemp() - 1);
        screenHeater();
      }
      else if (inBounds(x, y, 200, 109, 231, 142))
      {
        heater01.setOnTemp(heater01.getOnTemp() + 1);
        screenHeater();
      }
      else if (inBounds(x, y, 80, 150, 111, 183))
      {
        heater01.setWarnTemp(heater01.getWarnTemp() - 1);
        screenHeater();
      }
      else if (inBounds(x, y, 200, 150, 231, 183))
      {
        heater01.setWarnTemp(heater01.getWarnTemp() + 1);
        screenHeater();
      }
      break;

    case 9:
      if (inBounds(x, y, 0, 0, 119, 39))
      {
        screenHome();
      }
      else if (inBounds(x, y, 120, 0, 239, 39))
      {
        screenSettings();
      }
      else if (inBounds(x, y, 4, 64, 117, 97))
      {
        screenPwrSchedule();
      }
      else if (inBounds(x, y, 122, 64, 235, 97))
      {
        screenDosingSched();
      }
      
      break;

    case 10:
      if (inBounds(x, y, 0, 0, 119, 39))
      {
        drawPleaseWait();
        for (byte i = 0; i < numDosingPumps; i++)
        {
          dosingPumps[i].saveSettings();
        }

        screenHome();
      }
      else if (inBounds(x, y, 120, 0, 239, 39))
      {
        drawPleaseWait();
        for (byte i = 0; i < numDosingPumps; i++)
        {
          dosingPumps[i].saveSettings();
        }
                
        screenSettings();
      }
      else if (inBounds(x, y, 5, 65, 80, 96))
      {
        selectedItem = 0;
        drawDosingPumpTabs(selectedItem);
        drawDosingPumpSettings(selectedItem);
      } 
      else if (inBounds(x, y, 82, 65, 158, 96))
      {
        selectedItem = 1;
        drawDosingPumpTabs(selectedItem);
        drawDosingPumpSettings(selectedItem);
      } 
      else if (inBounds(x, y, 160, 65, 234, 96))
      {
        selectedItem = 2;
        drawDosingPumpTabs(selectedItem);
        drawDosingPumpSettings(selectedItem);
      }
      else if (inBounds(x, y, 80, 105, 111, 138))
      {
        dosingPumps[selectedItem].setDoseAmt(dosingPumps[selectedItem].getDoseAmt() - 1);
        drawDosingPumpSettings(selectedItem);
      }
      else if (inBounds(x, y, 200, 105, 231, 138))
      {
        dosingPumps[selectedItem].setDoseAmt(dosingPumps[selectedItem].getDoseAmt() + 1);
        drawDosingPumpSettings(selectedItem);
      }
      else if (inBounds(x, y, 80, 146, 111, 179))
      {
        dosingPumps[selectedItem].setMlSec(dosingPumps[selectedItem].getMlSec() - 10);
        drawDosingPumpSettings(selectedItem);
      }
      else if (inBounds(x, y, 200, 146, 231, 179))
      {
        dosingPumps[selectedItem].setMlSec(dosingPumps[selectedItem].getMlSec() + 10);
        drawDosingPumpSettings(selectedItem);
      }
      else if (inBounds(x, y, 80, 187, 111, 219))
      {
        dosingPumps[selectedItem].setVol(dosingPumps[selectedItem].getVol() - 10);
        drawDosingPumpSettings(selectedItem);
      }
      else if (inBounds(x, y, 200, 187, 231, 219))
      {
        dosingPumps[selectedItem].setVol(dosingPumps[selectedItem].getVol() + 10);
        drawDosingPumpSettings(selectedItem);
      }
      else if (inBounds(x, y, 8, 280, 118, 313))
      {
        dosingPumps[selectedItem].setRemainingVol(dosingPumps[selectedItem].getVol());
        drawDosingPumpSettings(selectedItem);
      }
      else if (inBounds(x, y, 120, 280, 230, 313))
      {
        drawPleaseWait();
        dosingPumps[selectedItem].dose();
        screenDosing(selectedItem);
      }
      break;

    case 11:
      if (inBounds(x, y, 80, 0, 165, 39))
      {     
        screenHome();
      }
      break;

     case 12:
      if (inBounds(x, y, 80, 0, 165, 39))
      {     
        screenHome();
      }
      break;
      
    case 13:
      if (inBounds(x, y, 80, 0, 165, 39))
      {     
        screenHome();
      }
      break;

    case 14:
      if (inBounds(x, y, 80, 0, 165, 39))
      {     
        screenHome();
      }
      break;

    case 15:
      if (inBounds(x, y, 0, 0, 119, 39))
      {
        drawPleaseWait();

        screenHome();
      }
      else if (inBounds(x, y, 120, 0, 239, 39))
      {
        drawPleaseWait();
        
        screenSettings();
      }  
      break;
      
    case 16:
      if (inBounds(x, y, 0, 0, 119, 39))
      {
        drawPleaseWait();
        
        display01.saveSettings();

        screenHome();
      }
      else if (inBounds(x, y, 120, 0, 239, 39))
      {
        drawPleaseWait();
        
        display01.saveSettings();
        
        screenSettings();
      }
      else if (inBounds(x, y, 80, 68, 111, 101))
      {
        display01.setHomeTimeout(display01.getHomeTimeout() - 1);
        screenScreen();
      }
      else if (inBounds(x, y, 200, 68, 231, 101))
      {
        display01.setHomeTimeout(display01.getHomeTimeout() + 1);
        screenScreen();
      }
      else if (inBounds(x, y, 80, 109, 111, 142))
      {
        display01.setDimLevel(display01.getDimLevel() - 1);
        screenScreen();
      }
      else if (inBounds(x, y, 200, 109, 231, 142))
      {
        display01.setDimLevel(display01.getDimLevel() + 1);
        screenScreen();
      }
      else if (inBounds(x, y, 80, 150, 111, 183))
      {
        display01.setDimSecs(display01.getDimSecs() - 1);
        screenScreen();
      }
      else if (inBounds(x, y, 200, 150, 231, 183))
      {
        display01.setDimSecs(display01.getDimSecs() + 1);
        screenScreen();
      }
      else if (inBounds(x, y, 80, 191, 111, 224))
      {
        display01.setBrightness(display01.getBrightness() - 1);
        screenScreen();
      }
      else if (inBounds(x, y, 200, 191, 231, 224))
      {
        display01.setBrightness(display01.getBrightness() + 1);
        screenScreen();
      }
      break;
   }
}

void check_Ph(boolean print)
{
  DEBUG_PRINTLN("Checking pH");
  
  float phVal = pH01.requestPh(dallasTemperatureSensors.getTempC(temperatureProbe01));

  if (relays[7].inSchedule())
  {
	  if (phVal >= pH01.getHighAlarmPh() && relays[7].isOff())
	  {
  	  relays[7].on();
	  }
	  else if (phVal <= pH01.getLowAlarmPh() && relays[7].isOff())
	  {
  	  relays[7].off();
	  }
  }

  if (print)
  {
    char phString[5] = "0.00";
    
    dtostrf(phVal, 2, 2, phString);
    
    if (String(phString) != String(prevphString))
    {
      myGLCD.setColor(THEME_PRIMARY_BACK.r, THEME_PRIMARY_BACK.g, THEME_PRIMARY_BACK.b);

      if (phString[0] != prevphString[0])
      {
        myGLCD.fillRect(140, 64, 162, 106);
      }
      
      if (phString[2] != prevphString[2])
      {
        myGLCD.fillRect(170, 64, 195, 106);
      }
      
      if (phString[3] != prevphString[3])
      {
        myGLCD.fillRect(195, 64, 220, 106);
      }
      
      utext.setForeground(88, 102, 106);
      utext.setFont(Arial30);
      utext.print(140, 70, phString);
      
      strncpy(prevphString, phString, 7);
    }
  }  
}

void check_Temperatures(boolean print)
{    
  DEBUG_PRINTLN("Checking temperature");
  
  if (millis() - lastTempRequest >= delayInMillis)
  {
    float probeTemp01 = dallasTemperatureSensors.getTempC(temperatureProbe01);
	  dallasTemperatureSensors.requestTemperatures();
    lastTempRequest = millis();
    
    dallasTemperatureSensors.processAlarms();

	  if (!dallasTemperatureSensors.hasAlarm(temperatureProbe01) && !heater01.isOn())
	  {
      heater01.on();
    }

	  //float probeTemp02 = dallasTemperatureSensors.getTempC(temperatureProbe02);

    if (print)
    {
      char tempstring[7];
 
      if (probeTemp01 == -127.0)
      {
        probeTemp01 = 0;
      }
  
      dtostrf(probeTemp01, 4, 1, tempstring);
  
      if (String(tempstring) != String(prevtempString))
      {
        myGLCD.setColor(THEME_PRIMARY_BACK.r, THEME_PRIMARY_BACK.g, THEME_PRIMARY_BACK.b);
        myGLCD.fillRect(4, 64, 117, 106);
    
        if (probeTemp01 >= heater01.getOffTemp())
        {
          utext.setForeground(255, 0, 0);
        }
        else if (probeTemp01 <= heater01.getWarnTemp())
        {
          utext.setForeground(0, 0, 255);
        }
        else
        {
          utext.setForeground(88, 102, 110);
        }
    
        utext.setFont(Arial30);

        utext.print(22, 70, tempstring);
    
        strncpy(prevtempString, tempstring, 7);
      }    
    }
  }  
}

void SaveTime()
{           
  saveRTC.tYear = (saveRTC.tYear + 2000);
  RTC.adjust(DateTime(saveRTC.tYear, saveRTC.tMonth, saveRTC.tDay, saveRTC.tHour, saveRTC.tMinute, saveRTC.tSecond));
  RTC.now().unixtime();
}

void updateTimeDate()
{
  RTC.now();

  if ((hour() != prevRTC.tHour) || (minute() != prevRTC.tMinute))
  {
    prevRTC.tHour = hour();
    prevRTC.tMinute = minute();
  
    int hr_12;
    byte ampm;
  
    hr_12 = prevRTC.tHour % 12;
  
    if (hr_12 == 0)
    {
      hr_12 = 12;
    }
    
    if (hour() <= 11)
    {
      ampm = 0;
    }
    else
    {
      ampm = 1;
    }
    
    printTime(hr_12, minute(), ampm, 180, 2);
  } 
}

void printTime(int thour, int tminute, byte ampm, int posx, int posy)
{
  char tmpTime[8];
  char charT[3];

  tmpTime[0] = '\0';

  if (thour >=0 && thour <= 9)
  {
    strcat(tmpTime, " ");
    itoa(thour, charT, 10);
    strcat(tmpTime, charT);
  }
  else
  { 
    itoa(thour, tmpTime, 10);
  }
  
  strcat(tmpTime, ":");  
  
  if (tminute >=0 && tminute <=9)
  {
    strcat(tmpTime, "0");
    itoa(tminute, charT, 10);
    strcat(tmpTime, charT);
  }
  else
  {
    itoa(tminute, charT, 10);
    strcat(tmpTime, charT);
  }
  
  if (ampm == 0)
  {
    strcat(tmpTime, "AM");
  }
  else
  {
    strcat(tmpTime, "PM");
  }
  
  myGLCD.setColor(246, 248, 248);
  myGLCD.fillRect(165, 42, 239, 59);
        
  utext.setForeground(88, 102, 110);
  utext.setBackground(246, 248, 248);
  utext.setFont(Arial11);
  utext.print(178, 44, tmpTime);
}

time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss)
{
  tmElements_t tmSet;

  tmSet.Year = YYYY - 1970;
  tmSet.Month = MM;
  tmSet.Day = DD;
  tmSet.Hour = hh;
  tmSet.Minute = mm;
  tmSet.Second = ss;

  return makeTime(tmSet);
}

void autoBrightness()
{
  int photocellReading = analogRead(lightSensorPin); 
  
  int brightnessLevel = map(photocellReading, 0, 1023, 0, 255);

  int brightAdjust;

  if (display01.getDimLevel() == 1)
  {
    brightAdjust =- 20;
  }
  else if (display01.getDimLevel() == 2)
  {
    brightAdjust =- 10;
  }
  else if (display01.getDimLevel() == 3)
  {
    brightAdjust = 0;
  }
  else if (display01.getDimLevel() == 4)
  {
    brightAdjust = 10;
  }
  else if (display01.getDimLevel() == 5)
  {
    brightAdjust = 20;
  }

  brightnessLevel = brightnessLevel + brightAdjust;
  
  if (brightnessLevel > 255)
  {
    brightnessLevel = 255;
  }
  
  if (brightnessLevel < 2)
  {
    brightnessLevel = 1;
  }
  
  int diff = brightnessLevel - backLight;
  
  if (diff < 0)
  {
      diff = ((diff - diff) - diff);
  }
  
  if (diff > 25)
  {
    rampScreenBrightness(backLight, brightnessLevel);
  }
}

void rampScreenBrightness(byte fromLevel, byte toLevel)
{
  if (fromLevel < toLevel)
  {
    while (fromLevel < toLevel) 
    {
      analogWrite(screenBrightnessPin, fromLevel);

      fromLevel++;

      delay(5);
    }
  }
  else if (fromLevel > toLevel)
  {
    while (fromLevel > toLevel) 
    {
      analogWrite(screenBrightnessPin, fromLevel);  

      fromLevel--;

      delay(5);
    }
  }
  
  backLight = toLevel;
}
 
void update_alarms()
{
  DEBUG_PRINTLN("Updating dosing pump alarms");
  
  for (byte i = 0; i < numDosingPumps; i++)
  {
    dosingPumps[i].updateAlarms();
  }

  DEBUG_PRINTLN("Updating relay alarms");

  for (byte i = 0; i <= numRelays; i++)
  {
    relays[i].updateAlarms();
  }
}

void alarm_temperature(const uint8_t* deviceAddress)
{  
  float probeTemp = dallasTemperatureSensors.getTempC(deviceAddress);
  
  if (probeTemp >= dallasTemperatureSensors.getHighAlarmTemp(deviceAddress) & heater01.isOn())
  {
    DEBUG_PRINTLN("Triggering high temperature alarm");
    heater01.off();
  }
  else if (probeTemp <= dallasTemperatureSensors.getLowAlarmTemp(deviceAddress))
  {
    DEBUG_PRINTLN(probeTemp);
    DEBUG_PRINTLN(dallasTemperatureSensors.getLowAlarmTemp(deviceAddress));
    DEBUG_PRINTLN("Triggering low temperature alarm");
    buzzer.beep(1, 100);
  }
}

void alarm_rlyLight1_on() 
{
  relays[0].on();
}

void alarm_rlyLight2_on()
{
  relays[1].on();
}

void alarm_rlyFilter1_on()
{
  relays[2].on();
}

void alarm_rlyFilter2_on()
{
  relays[3].on();
}

void alarm_rlyHeater1_on()
{
  relays[4].on();
}

void alarm_rlyHeater2_on()
{
  relays[5].on();
}

void alarm_rlyCirc_on()
{
  relays[6].on();
}

void alarm_rlyCO2_on()
{
  relays[7].on();
}

void alarm_rlyLight1_off()
{
  relays[0].off();
}

void alarm_rlyLight2_off()
{
  relays[1].off();
}

void alarm_rlyFilter1_off()
{
  relays[2].off();
}

void alarm_rlyFilter2_off()
{
  relays[3].off();
}

void alarm_rlyHeater1_off()
{
  relays[4].off();
}

void alarm_rlyHeater2_off()
{
  relays[5].off();
}

void alarm_rlyCirc_off()
{
  relays[6].off();
}

void alarm_rlyCO2_off()
{
  relays[7].off();
}

void alarm_macro_dose()
{
  dosingPumps[0].dose();
}

void alarm_micro_dose()
{
  dosingPumps[1].dose();
}

void alarm_glut_dose()
{
  dosingPumps[3].dose();
}

void firstRunSetup()
{
  if (EEPROM.readByte(0) < 1) 
  {
    EEPROM.writeByte(0,1);

    EEPROM.writeByte(1,1); // Relay 1 Active
    EEPROM.writeByte(2,11); // Relay 1 On Hour
    EEPROM.writeByte(3,0); // Relay 1 On Minute
    EEPROM.writeByte(4,19); // Relay 1 Off Hour
    EEPROM.writeByte(5,0); // Relay 1 Off Minute

    EEPROM.writeByte(8,1); // Relay 2 Active
    EEPROM.writeByte(9,19); // Relay 2 On Hour
    EEPROM.writeByte(10,0); // Relay 2 On Minute
    EEPROM.writeByte(11,21); // Relay 2 Off Hour
    EEPROM.writeByte(12,30); // Relay 2 Off Minute

    EEPROM.writeByte(15,0); // Relay 3 Active
    EEPROM.writeByte(16,0); // Relay 3 On Hour
    EEPROM.writeByte(17,0); // Relay 3 On Minute
    EEPROM.writeByte(18,0); // Relay 3 Off Hour
    EEPROM.writeByte(19,0); // Relay 3 Off Minute

    EEPROM.writeByte(22,0); // Relay 4 Active
    EEPROM.writeByte(23,0); // Relay 4 On Hour
    EEPROM.writeByte(24,0); // Relay 4 On Minute
    EEPROM.writeByte(25,0); // Relay 4 Off Hour
    EEPROM.writeByte(26,0); // Relay 4 Off Minute

    EEPROM.writeByte(29,0); // Relay 5 Active
    EEPROM.writeByte(30,0); // Relay 5 On Hour
    EEPROM.writeByte(31,0); // Relay 5 On Minute
    EEPROM.writeByte(32,0); // Relay 5 Off Hour
    EEPROM.writeByte(33,0); // Relay 5 Off Minute

    EEPROM.writeByte(36,0); // Relay 6 Active
    EEPROM.writeByte(37,0); // Relay 6 On Hour
    EEPROM.writeByte(38,0); // Relay 6 On Minute
    EEPROM.writeByte(39,0); // Relay 6 Off Hour
    EEPROM.writeByte(40,0); // Relay 6 Off Minute

    EEPROM.writeByte(43,1); // Relay 7 Active
    EEPROM.writeByte(43,0); // Relay 7 On Hour
    EEPROM.writeByte(43,0); // Relay 7 On Minute
    EEPROM.writeByte(43,0); // Relay 7 Off Hour
    EEPROM.writeByte(43,0); // Relay 7 Off Minute

    EEPROM.writeByte(50,1); // Relay 8 Active
    EEPROM.writeByte(51,10); // Relay 8 On Hour
    EEPROM.writeByte(52,30); // Relay 8 On Minute
    EEPROM.writeByte(53,18); // Relay 8 Off Hour
    EEPROM.writeByte(54,30); // Relay 8 Off Minute
  }

  if (EEPROM.readByte(57) < 1)
  {
    EEPROM.writeByte(57,1);

    EEPROM.writeByte(58,1); // Pump 1 Active
    EEPROM.writeByte(59,10); // Pump 1 On Hour
    EEPROM.writeByte(60,30); // Pump 1 On Minute
    EEPROM.writeByte(61,1); // Pump 1 On Sunday
    EEPROM.writeByte(62,0); // Pump 1 On Monday
    EEPROM.writeByte(63,1); // Pump 1 On Tuesday
    EEPROM.writeByte(64,0); // Pump 1 On Wednesday
    EEPROM.writeByte(65,1); // Pump 1 On Thursday
    EEPROM.writeByte(66,0); // Pump 1 On Friday
    EEPROM.writeByte(67,0); // Pump 1 On Saturday
    EEPROM.writeByte(68,5); // Pump 1 Dose
    EEPROM.writeByte(69,122); // Pump 1 mL/s
    EEPROM.writeByte(70,99); // Pump 1 Volume
    EEPROM.writeInt(71,0); // Pump 1 Remaining Volume

    EEPROM.writeByte(75,1); // Pump 2 Active
    EEPROM.writeByte(76,10); // Pump 2 On Hour
    EEPROM.writeByte(77,30); // Pump 2 On Minute
    EEPROM.writeByte(78,0); // Pump 2 On Sunday
    EEPROM.writeByte(79,1); // Pump 2 On Monday
    EEPROM.writeByte(80,0); // Pump 2 On Tuesday
    EEPROM.writeByte(81,1); // Pump 2 On Wednesday
    EEPROM.writeByte(82,0); // Pump 2 On Thursday
    EEPROM.writeByte(83,1); // Pump 2 On Friday
    EEPROM.writeByte(84,0); // Pump 2 On Saturday
    EEPROM.writeByte(85,5); // Pump 2 Dose
    EEPROM.writeByte(86,122); // Pump 2 mL/s
    EEPROM.writeByte(87,99); // Pump 2 Volume
    EEPROM.writeInt(88,0); // Pump 2 Remaining Volume

    EEPROM.writeByte(92,1); // Pump 3 Active
    EEPROM.writeByte(93,15); // Pump 3 On Hour
    EEPROM.writeByte(94,0); // Pump 3 On Minute
    EEPROM.writeByte(95,0); // Pump 3 On Sunday
    EEPROM.writeByte(96,1); // Pump 3 On Monday
    EEPROM.writeByte(97,0); // Pump 3 On Tuesday
    EEPROM.writeByte(98,1); // Pump 3 On Wednesday
    EEPROM.writeByte(99,0); // Pump 3 On Thursday
    EEPROM.writeByte(100,1); // Pump 3 On Friday
    EEPROM.writeByte(101,0); // Pump 3 On Saturday
    EEPROM.writeByte(102,5); // Pump 3 Dose
    EEPROM.writeByte(103,122); // Pump 3 mL/s
    EEPROM.writeByte(104,99); // Pump 3 Volume
    EEPROM.writeInt(105,0); // Pump 3 Remaining Volume

    EEPROM.writeByte(109,0); // Pump 4 Active
    EEPROM.writeByte(110,0); // Pump 4 On Hour
    EEPROM.writeByte(111,0); // Pump 4 On Minute
    EEPROM.writeByte(112,0); // Pump 4 On Sunday
    EEPROM.writeByte(113,0); // Pump 4 On Monday
    EEPROM.writeByte(114,0); // Pump 4 On Tuesday
    EEPROM.writeByte(115,0); // Pump 4 On Wednesday
    EEPROM.writeByte(116,0); // Pump 4 On Thursday
    EEPROM.writeByte(117,0); // Pump 4 On Friday
    EEPROM.writeByte(118,0); // Pump 4 On Saturday
    EEPROM.writeByte(119,0); // Pump 4 Dose
    EEPROM.writeByte(120,0); // Pump 4 mL/s
    EEPROM.writeByte(121,0); // Pump 4 Volume
    EEPROM.writeInt(122,0); // Pump 4 Remaining Volume
  }

  if (EEPROM.readByte(126) < 1)
  {
    EEPROM.writeByte(126,1);

    EEPROM.writeFloat(127,7.2); // Atlas pH High Alarm
    EEPROM.writeFloat(131,5.6); // Atlas pH Low Alarm
  }

  if (EEPROM.readByte(137) < 1)
  {
    EEPROM.writeByte(137,1);

    EEPROM.writeByte(138,30); // Heater Off Temp
    EEPROM.writeByte(139,25); // Heater On Temp
    EEPROM.writeByte(140,22); // Heater Low Temp Alarm
  }

  if (EEPROM.readByte(143) < 1)
  {
    EEPROM.writeByte(143,1);

    EEPROM.writeByte(144,30); // Screen Return Home
    EEPROM.writeByte(145,25); // Screen Auto-dim Level
    EEPROM.writeByte(146,5); // Screen Auto-dim Seconds 
    EEPROM.writeByte(147,255); // Screen Brightness
  }
}