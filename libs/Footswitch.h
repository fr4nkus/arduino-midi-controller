/* beeb boop beep boop */

#ifndef Footswitch_h
#define Footswitch_h

#include "Arduino.h"

class Footswitch
{
  public:
    Footswitch(int pin);
    int read_pin();
    bool is_pressed();
    bool is_changed();
    bool hold();
    void refresh();
    bool is_on();
    
  private:
    int _pin;
    int _status;
    int _old_status;
    bool _activated;
    bool _changed;
    bool _latch;
    bool _read;
};

#endif
