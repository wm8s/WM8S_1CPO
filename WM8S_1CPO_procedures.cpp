#include <Arduino.h>
#include <WM8S_Morse.h>

#include "WM8S_1CPO.h"

////////////////////////////////////////////////////////////////////////////////
//
//  1CPO - My First Code Practice Oscillator. Like 3CPO, only mintier.
//
//	Copyright © 2016 Robert L. Bailey
//
////////////////////////////////////////////////////////////////////////////////
//
//  Helper Procedures
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

void debouncePB()
{
  // delay long enough for the switch to debounce:
  
  delay(WM8S_1CPO_DEBOUNCE_TIME);
}

void waitForPulledupButtonRelease(WM8S::pin_t pPin)
{
  // wait for the switch to settle down:
  
  debouncePB();

  // and him to let up on the switch:
  
  while (isPulledupButtonPressed(pPin)) {};
}

bool isPulledupButtonPressed(WM8S::pin_t pPin)
{
  // return true if passed INPUT_PULLUP PB is pressed:
  
  return (digitalRead(pPin) == LOW);
}

void initIO()
{
  // setup I/O, etc.:
  
  pinMode(WM8S_1CPO_PB1, INPUT_PULLUP);
  pinMode(WM8S_1CPO_PB2, INPUT_PULLUP);
  pinMode(WM8S_1CPO_PB3, INPUT_PULLUP);
}

void handleReset()
{
  // if PB1 & PB2 are pressed when we power up, reset settings:

  if (isPulledupButtonPressed(WM8S_1CPO_PB1) && 
  		isPulledupButtonPressed(WM8S_1CPO_PB2))
  {
    beep();
    
    // wait for him to release both buttons:
    
    while (isPulledupButtonPressed(WM8S_1CPO_PB1) ||
    			 isPulledupButtonPressed(WM8S_1CPO_PB2)) {}
    
    WM8S_1CPO.resetSettings();
  }
}

void beep()
{
	// interesting beep

	delay(WM8S_1CPO_TONE_HI_0_DURATION);
	for (byte i = 0; i < WM8S_1CPO_TONE_HI_1_LOOPS; i++)
	{
		tone(WM8S_1CPO.tonePin(), WM8S_1CPO_TONE_HI_1_HZ);
		delay(WM8S_1CPO_TONE_HI_1_DURATION);
		tone(WM8S_1CPO.tonePin(), WM8S_1CPO_TONE_HI_2_HZ);
		delay(WM8S_1CPO_TONE_HI_2_DURATION);
	}
	noTone(WM8S_1CPO.tonePin());
	delay(WM8S_1CPO_TONE_HI_3_DURATION);
}

void startMorse()
{
  // start:

  WM8S_1CPO.begin(WM8S_1CPO_TONEPIN);

  WM8S_1CPO.setRunMode(sending ? sendingWhat : WM8S_Morse::Idle);
}

void sendSettingMode()
{
  // send the current setting mode in CW:
    
  char bufr[WM8S_1CPO_MAXSETTINGCFMBUFRSIZE];
  bufr[0] = settingMode;
  bufr[1] = WM8S_END_OF_STRING;

  WM8S::hz_t oldPitch = WM8S_1CPO.pitch();
  WM8S_1CPO.setPitch(WM8S_1CPO.pitch() + WM8S_1CPO_PITCHOFFSET_SETTINGMODE, false);
  WM8S_1CPO.sendMessage(bufr);
  WM8S_1CPO.setPitch(oldPitch, false);
}

void sendSettingValue()
{
  // send current setting mode's value:
  
  char bufr[WM8S_1CPO_MAXSETTINGVALUECFMBUFRSIZE];   

  switch (settingMode)
  {
    case SendWhat:
      switch (sendingWhat)
      {
        case WM8S_Morse::SendQSOs:
          bufr[0] = WM8S_1CPO_SendWhat_Mode::SendQSOs;
          break;

        case WM8S_Morse::SendGroups:
          bufr[0] = WM8S_1CPO_SendWhat_Mode::SendGroups;
          break;

        case WM8S_Morse::SendChars:
          bufr[0] = WM8S_1CPO_SendWhat_Mode::SendChars;
          break;

        default:    // huh?
          bufr[0] = WM8S_1CPO_SendWhat_Mode::Unknown;
          break;
      }
      bufr[1] = WM8S_END_OF_STRING;
      break;
      
    case OverallSpeed:
      sprintf(bufr, WM8S_1CPO_SETTINGVALUE_FORMAT_OVERALLSPEED, WM8S_1CPO.overallSpeed());
      break;

    case Difficulty:
      sprintf(bufr, WM8S_1CPO_SETTINGVALUE_FORMAT_DIFFICULTY, WM8S_1CPO.enabledDifficulty());
      break;

    case Pitch:
      sprintf(bufr, WM8S_1CPO_SETTINGVALUE_FORMAT_PITCH, (WM8S_1CPO.pitch() / 100));
      break;

    case CharSpeed:
      sprintf(bufr, WM8S_1CPO_SETTINGVALUE_FORMAT_CHARSPEED, WM8S_1CPO.userCharSpeed());
      break;
  }
  WM8S_1CPO.sendMessage(bufr);
}

void sendSettingModeAndValue()
{
  // send setting mode and value:
    
  sendSettingMode();
  sendSettingValue();
}

WM8S_Morse::wpm_t nextSpeed(WM8S_Morse::wpm_t pSpeed)
{
  // return the next speed in the array of discrete speeds
  // that is greater than the passed speed

  byte p = 0;
  byte numVals = WM8S_ARRAY_SIZE(WM8S_1CPO_Speeds);

  // get the index in the array of the biggest element <= the passed value,
  // or 0 if < the lower item:
  
  for (byte i = 0; i < numVals; i++)
  {
    if (WM8S_1CPO_Speeds[i] <= pSpeed) p = i;
  }

  // bump and wrap:

  p++; if (p >= numVals) p = 0;

  return WM8S_1CPO_Speeds[p];
}

WM8S::hz_t nextPitch(WM8S::hz_t pPitch)
{
  // return the next pitch in the array of discrete pitches
  // that is greater than the passed pitch

  byte p = 0;
  byte numVals = WM8S_ARRAY_SIZE(WM8S_1CPO_Pitches);

  // get the index in the array of the biggest element <= the passed value,
  // or 0 if < the lower item:
  
  for (byte i = 0; i < numVals; i++)
  {
    if (WM8S_1CPO_Pitches[i] <= pPitch) p = i;
  }

  // bump and wrap:

  p++; if (p >= numVals) p = 0;

  return WM8S_1CPO_Pitches[p];
}


WM8S_Morse::difficulty_t nextDifficulty(WM8S_Morse::difficulty_t pDifficulty)
{
  // return the next difficulty in the array of discrete difficulties
  // that is greater than the passed difficulty

  byte p = 0;
  byte numVals = WM8S_ARRAY_SIZE(WM8S_1CPO_Difficulties);

  // get the index in the array of the biggest element <= the passed value,
  // or 0 if < the lower item:
  
  for (byte i = 0; i < numVals; i++)
  {
    if (WM8S_1CPO_Difficulties[i] <= pDifficulty) p = i;
  }

  // bump and wrap:

  p++; if (p >= numVals) p = 0;

  return WM8S_1CPO_Difficulties[p];
}

void rotateSettingMode()
{
  // rotate setting mode

  switch (settingMode)
  {
    case SendWhat:
      settingMode = OverallSpeed;
      break;
      
    case OverallSpeed:
      settingMode = Difficulty;
      break;

    case Difficulty:
      settingMode = Pitch;
      break;

    case Pitch:
      settingMode = CharSpeed;
      break;

    case CharSpeed:
    default:
      settingMode = SendWhat;
      break;
  }
  sendSettingModeAndValue();
}

void rotateSendWhat()
{
  // rotate run mode:
  
  switch (sendingWhat)
  {
    case WM8S_Morse::SendQSOs:
      sendingWhat = WM8S_Morse::SendGroups;
      break;
      
    case WM8S_Morse::SendGroups:
      sendingWhat = WM8S_Morse::SendChars;
      break;

    case WM8S_Morse::SendChars:
      sendingWhat = WM8S_Morse::SendQSOs;
      break;

    default: // shouldn't be here
    case WM8S_Morse::Idle:
      sendingWhat = WM8S_Morse::SendQSOs;
      break;
  }
}

void rotateOverallSpeed()
{
  // rotate Overall Speed:

  WM8S_1CPO.setOverallSpeed(nextSpeed(WM8S_1CPO.overallSpeed()));
}

void rotatePitch()
{
  // rotate pitch:
  
  WM8S_1CPO.setPitch(nextPitch(WM8S_1CPO.pitch()), true);
}

void rotateCharacterSpeed()
{
  // rotate Character Speed:  

  WM8S_1CPO.setUserCharSpeed(nextSpeed(WM8S_1CPO.userCharSpeed()));
}

void rotateDifficulty()
{
  // rotate difficulty:
  
  WM8S_1CPO.setEnabledDifficulty(nextDifficulty(WM8S_1CPO.enabledDifficulty()));
}

void rotateSettingValue()
{
  // rotate setting value of current setting mode

  switch (settingMode)
  {
    case SendWhat:
      rotateSendWhat();
      break;
      
    case OverallSpeed:
      rotateOverallSpeed();
      break;

    case Difficulty:
      rotateDifficulty();
      break;

    case Pitch:
      rotatePitch();
      break;

    case CharSpeed:
      rotateCharacterSpeed();
      break;
  }
  sendSettingValue();
}

void handlePB1Pressed()
{
  // toggles sending flag:
  
  beep();
  waitForPulledupButtonRelease(WM8S_1CPO_PB1);

  sending = !sending;
  
  if (sending)
  {
    WM8S_1CPO.setRunMode(sendingWhat);
  }
  else
  {
    WM8S_1CPO.setRunMode(WM8S_Morse::Idle);
    sendSettingModeAndValue();
  }
}

void handlePB2Pressed()
{
  // in Idle
  //    rotate through setting modes
  // else
  //    rotate through Overall Speeds
  
  beep();
  waitForPulledupButtonRelease(WM8S_1CPO_PB2);

  if (sending)
  {
    // rotate Overall speed:
    rotateOverallSpeed();
  }
  else
  {
    // rotate setting mode:
    rotateSettingMode();
  }
}

void handlePB3Pressed()
{
  // in Idle
  //    rotate through setting values
  // else
  //    rotate through Pitches
  
  beep();
  waitForPulledupButtonRelease(WM8S_1CPO_PB3);

  if (sending)
  {
    // rotate pitch:
    rotatePitch();
  }
  else
  {
    // rotate settings value for current mode:
    rotateSettingValue();
  }
}
