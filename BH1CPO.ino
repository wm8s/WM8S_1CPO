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
//  You can [probably] safely change anything marked with /// CUSTOMIZE: tag.
//  You probably shouldn't change anything else unless you understand it well.
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
  Serial.flush();
#endif

  initIO();
  handleReset();
  startMorse();
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
