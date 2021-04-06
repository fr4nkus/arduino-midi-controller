#include "Arduino.h"
#include "footswitch.h"


Footswitch::Footswitch(int pin)
{
  pinMode(pin, INPUT);
  _status = LOW;
  _old_status = LOW;
  _pin = pin;
  _activated = false;
  _latch = false;
}


int Footswitch::read_pin()
{
  if( !_read ) {
    _old_status = _status;
    _status = digitalRead( _pin ); 
    _read = true;
  }
  return _status;
}


bool Footswitch::is_pressed()
{
  _activated = read_pin() == HIGH;
  return _activated;
}


bool Footswitch::is_changed()
{
  return _old_status != read_pin();
}


bool Footswitch::hold( )
{
  if( is_pressed() && is_changed() ) {
    _latch = !_latch;
    return true;
  }
  return false;
}


void Footswitch::refresh()
{
  _read = false;
}


bool Footswitch::is_on()
{
  return _latch;
}
