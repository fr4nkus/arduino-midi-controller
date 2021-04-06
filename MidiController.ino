/*
  Eventide Timefactor / DryBell Controller 
  Eventide Timefactor Delay Pedal:
    bank up (momentary)
    bank down (momentary)
    expression (momentary) CC0-CC99
    expression (latching) CC0-CC99
  DryBell Univibe Pedal
    expression (latching) control voltage 0-5v on the tip
*/

#include <Footswitch.h>
#include <MIDI.h>


// #define DEBUG 1

#ifdef DEBUG
  /* Logging and MIDI use the same tx so are mutually exclusive 
   * To check the logic use the system messages to review the actions
   */
#else
  MIDI_CREATE_DEFAULT_INSTANCE();
#endif

// The Arduino ports for footswitches
Footswitch bankUp( 4 );
Footswitch expression( 2 );
Footswitch bankDown( 3 );

/* expression selector switch:
 * Three position switch with central common - when the 
 * middle option is selected - both are on. 
*/ 
#define SWITCH_TIMEFACTOR_RECV_PIN 5
#define SWITCH_VIBEMACHINE_RECV_PIN 6

// pin for control voltage
#define VIBEMACHINE_TRIGGER_PIN 7

#define MIDI_PORT 4
#define MIDI_EXPRESSION_CC 11
#define MIDI_BANK_UP_CC 96
#define MIDI_BANK_DOWN_CC 97


void setup() {

  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println( "initialise the expression directer toggle switch" );
  #else
    MIDI.begin( MIDI_PORT );
  #endif
  
  pinMode( SWITCH_VIBEMACHINE_RECV_PIN, INPUT ); 
  pinMode( SWITCH_TIMEFACTOR_RECV_PIN, INPUT );
  pinMode( VIBEMACHINE_TRIGGER_PIN, OUTPUT );
}


void loop() {
  
  /* low-tech button debounce */
  delay( 20 );
  
  bankUp.refresh();
  bankDown.refresh();
  expression.refresh();
  
  // Test for a change of state in the pedals, allows for the removal of a foot ;^)
  if ( !( bankDown.is_changed() || bankUp.is_changed() || expression.is_changed() ) ) {
    return;
  }

  int pressedButtonFlags = ( 
    ( bankUp.is_pressed() ? 1 : 0 ) +
    ( bankDown.is_pressed() ? 2 : 0 ) +
    ( expression.is_pressed() ? 4 : 0 )
  );

  // Only act on single footswitch actions
  switch ( pressedButtonFlags ) {
    case 1: // bankUp.hold()
    
      #ifdef DEBUG
        Serial.println( "MIDI.sendProgramChange( MIDI_PC_UP, MIDI_PORT );" );
      #else
        MIDI.sendControlChange( MIDI_BANK_UP_CC, 1, MIDI_PORT );
      #endif
      
      delay( 250 );
      break;
      
    case 2: // bankDown.hold();

      #ifdef DEBUG
        Serial.println( "MIDI.sendProgramChange( MIDI_PC_DOWN, MIDI_PORT );" );
      #else
        MIDI.sendControlChange( MIDI_BANK_DOWN_CC, 1, MIDI_PORT );
      #endif
      
      delay( 250 );
      break;

    case 4: 
      expression.hold();
      
      #ifdef DEBUG
        if ( expression.is_on() ) {
          Serial.println( "exp status is on" );
        }
        else {
          Serial.println( "exp status is off" );
        }
      #endif
      
      if ( !digitalRead( SWITCH_TIMEFACTOR_RECV_PIN ) ) {

        #ifdef DEBUG
          Serial.println( "MIDI.sendControlChange( MIDI_EXPRESSION_CC, ( expression.is_on() ) ? 127 : 0, MIDI_PORT );" );
        #else
          MIDI.sendControlChange( MIDI_EXPRESSION_CC, ( expression.is_on() ) ? 127 : 0, MIDI_PORT );
        #endif 
        
        delay( 250 );
      }

      if ( !digitalRead(SWITCH_VIBEMACHINE_RECV_PIN) ) {

        #ifdef DEBUG
          Serial.println( "digitalWrite( VIBEMACHINE_TRIGGER_PIN, ( expression.is_on() )? HIGH : LOW );" );
        #else
          digitalWrite( VIBEMACHINE_TRIGGER_PIN, ( expression.is_on() )? HIGH : LOW ); 
        #endif 
        
        delay( 250 );
      } 
      break;
  }
  
} // end void loop
