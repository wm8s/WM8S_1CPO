#include <Arduino.h>

#include "BH1CPO.h"

////////////////////////////////////////////////////////////////////////////////
//
//  1CPO - My First Code Practice Oscillator. Like 3CPO, only mintier.
//
//	Copyright © 2016 Robert L. Bailey
//
////////////////////////////////////////////////////////////////////////////////
//
//  Main sketch
//
//  You can [probably] safely change anything marked with /// CUSTOMIZE: tag.
//  Anything, and you're on your own.
//
//////////////////////////////////////////////////////////////////////////////
//
//	This file is part of 1CPO.
//
//	1CPO is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	1CPO is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with 1CPO.  If not, see <http://www.gnu.org/licenses/>.
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
