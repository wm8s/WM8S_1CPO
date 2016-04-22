#include <Arduino.h>
#include <BHMorse.h>

#include "BH1CPO.h"

////////////////////////////////////////////////////////////////////////////////
//
//  1CPO - My First Code Practice Oscillator. Like 3CPO, only mintier.
//
//  Written by: Rob Bailey, WM8S
//
////////////////////////////////////////////////////////////////////////////////
//
//  Main sketch
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//  Global variables
//
////////////////////////////////////////////////////////////////////////////////

// the main CW sending object:

BHMorse BH_1CPO;

// sending or idle?

bool sending = true;

// if sending, sending what?

BHMorse::RunMode sendingWhat = BHMorse::SendQSOs;

// if idle, setting what?

BH_1CPO_Setting_Mode settingMode = BH_1CPO_DEFAULT_SETTING_MODE;

////////////////////////////////////////////////////////////////////////////////
//
// setup()
//
// do everything after constructors called, hardware initialized, etc
//
////////////////////////////////////////////////////////////////////////////////

void setup()
{
#ifdef BHMORSE_DEBUG
  Serial.begin(9600);
#endif

  // setup I/O, etc.:
  
  pinMode(BH_1CPO_PB1, INPUT_PULLUP);
  pinMode(BH_1CPO_PB2, INPUT_PULLUP);
  pinMode(BH_1CPO_PB3, INPUT_PULLUP);

  BH_1CPO.begin(BH_1CPO_TONEPIN);
  
  // if PB1 & PB2 are pressed when we come on, reset settings:

  if (isPulledupButtonPressed(BH_1CPO_PB1) && isPulledupButtonPressed(BH_1CPO_PB2))
  {
    BH_1CPO.beepHi();
    
    // wait for him to release both buttons:
    
    while (isPulledupButtonPressed(BH_1CPO_PB1) || isPulledupButtonPressed(BH_1CPO_PB2)) {}
    
    BH_1CPO.resetSettings();
  }

  // start:
  
  if (sending) BH_1CPO.setRunMode(sendingWhat);
  else BH_1CPO.setRunMode(BHMorse::Idle);
}

////////////////////////////////////////////////////////////////////////////////
//
// loop()
//
// do all my work in tiny nudges
//
////////////////////////////////////////////////////////////////////////////////

void loop()
{
  // handle button presses:
  
  if (isPulledupButtonPressed(BH_1CPO_PB1)) handlePB1Pressed();
  if (isPulledupButtonPressed(BH_1CPO_PB2)) handlePB2Pressed();
  if (isPulledupButtonPressed(BH_1CPO_PB3)) handlePB3Pressed();

  // send next element if there is one:
  
  if (!BH_1CPO.msgDone()) BH_1CPO.sendNextElem();
}
