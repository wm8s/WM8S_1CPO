#include <Arduino.h>

#include "WM8S_1CPO.h"

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

WM8S_Morse WM8S_1CPO;

// sending or idle?

bool sending = true;

// if sending, sending what?

WM8S_Morse::RunMode sendingWhat = WM8S_Morse::SendQSOs;

// if idle, setting what?

WM8S_1CPO_Setting_Mode settingMode = WM8S_1CPO_DEFAULT_SETTING_MODE;

////////////////////////////////////////////////////////////////////////////////
//
// setup()
//
// do everything after constructors called, hardware initialized, etc
//
////////////////////////////////////////////////////////////////////////////////

void setup()
{
#ifdef WM8S_MORSE_DEBUG
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
  
  if (isPulledupButtonPressed(WM8S_1CPO_PB1)) handlePB1Pressed();
  if (isPulledupButtonPressed(WM8S_1CPO_PB2)) handlePB2Pressed();
  if (isPulledupButtonPressed(WM8S_1CPO_PB3)) handlePB3Pressed();

  // send next element if there is one:
  
  if (!WM8S_1CPO.msgDone()) WM8S_1CPO.sendNextElem();
}
