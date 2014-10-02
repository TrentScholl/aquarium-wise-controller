#include "pins.h"
#include "buzzer.h"

void Buzzer::init(void)
{
  pinMode(alarmPin, OUTPUT);
}

void Buzzer::beep(byte numBeeps, int duration)
{
  for (byte beepCount = 0; beepCount < numBeeps; beepCount++)
  {
    digitalWrite(alarmPin, HIGH);
    delay(duration);
    digitalWrite(alarmPin, LOW);
    
    if (beepCount < numBeeps - 1)
    {
      delay(duration);
    }
  }
}

Buzzer buzzer;