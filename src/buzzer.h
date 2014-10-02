#pragma once

#include "Arduino.h"

class Buzzer
{
  public:
    Buzzer(){};
    ~Buzzer(){};
  
    void init(void);

    void beep(byte numBeeps, int duration);
};

extern Buzzer buzzer;