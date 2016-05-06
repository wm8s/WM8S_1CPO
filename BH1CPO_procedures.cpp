#include <Arduino.h>
#include <BHMorse.h>

#include "BH1CPO.h"

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

void initIO()
{
  // setup I/O, etc.:
  
  pinMode(BH_1CPO_PB1, INPUT_PULLUP);
  pinMode(BH_1CPO_PB2, INPUT_PULLUP);
  pinMode(BH_1CPO_PB3, INPUT_PULLUP);
}

void handleReset()
{
  // if PB1 & PB2 are pressed when we come on, reset settings:

  /// CUSTOMIZE: whichever buttons you want
  if (isPulledupButtonPressed(BH_1CPO_PB1) &&
  isPulledupButtonPressed(BH_1CPO_PB2))
  {
    beep();
    
    // wait for him to release both buttons:
    
    while (isPulledupButtonPressed(BH_1CPO_PB1) ||
    isPulledupButtonPressed(BH_1CPO_PB2)) {}
    
    BH_1CPO.resetSettings();
  }
}

void beep()
{
	// beep

	for (byte i = 0; i < BH_1CPO_TONE_HI_1_LOOPS; i++)
	{
		tone(BH_1CPO.tonePin(), BH_1CPO_TONE_HI_1_HZ);
		delay(BH_1CPO_TONE_HI_1_DURATION);
		tone(BH_1CPO.tonePin(), BH_1CPO_TONE_HI_2_HZ);
		delay(BH_1CPO_TONE_HI_2_DURATION);
		noTone(BH_1CPO.tonePin());
	}
	delay(BH_1CPO_TONE_HI_3_DURATION);
}

void startMorse()
{
  // start:

  BH_1CPO.begin(BH_1CPO_TONEPIN);
  
  if (sending) BH_1CPO.setRunMode(sendingWhat);
  else BH_1CPO.setRunMode(BHMorse::Idle);
}

void debouncePB()
{
  // delay long enough for the switch to debounce:
  
  delay(BH_1CPO_DEBOUNCE_TIME);
}

void waitForPulledupButtonRelease(BH_PIN pPin)
{
  // wait for him to let up on the switch:
  
  debouncePB();
  while (isPulledupButtonPressed(pPin)) {};
}

bool isPulledupButtonPressed(BH_PIN pPin)
{
  // return true if passed INPUT_PULLUP PB is pressed:
  
  return (digitalRead(pPin) == LOW);
}

void sendSettingMode()
{
  // send the current setting mode in CW:
    
  char bufr[BH_1CPO_MAXSETTINGCFMBUFRSIZE];
  bufr[0] = settingMode;
  bufr[1] = '\0';

  BHMorse_Hz oldPitch = BH_1CPO.pitch();
  BH_1CPO.setPitch(BH_1CPO.pitch() + BH_1CPO_PITCHOFFSET_SETTINGMODE, false);
  BH_1CPO.sendMessage(bufr);
  BH_1CPO.setPitch(oldPitch, false);
}

void sendSettingValue()
{
  // send current setting mode's value:
  
  char bufr[BH_1CPO_MAXSETTINGVALUECFMBUFRSIZE];   

  switch (settingMode)
  {
    case SendWhat:
      switch (sendingWhat)
      {
        case BHMorse::SendQSOs:
          bufr[0] = BH_1CPO_SendWhat_Mode::SendQSOs;
          break;

        case BHMorse::SendGroups:
          bufr[0] = BH_1CPO_SendWhat_Mode::SendGroups;
          break;

        case BHMorse::SendChars:
          bufr[0] = BH_1CPO_SendWhat_Mode::SendChars;
          break;

        default:    // huh?
          bufr[0] = BH_1CPO_SendWhat_Mode::Unknown;
          break;
      }
      bufr[1] = '\0';
      break;
      
    case OverallSpeed:
      sprintf(bufr, "%i", BH_1CPO.overallSpeed());
      break;

    case Difficulty:
      sprintf(bufr, "%i", BH_1CPO.highestEnabledGroup());
      break;

    case Pitch:
      sprintf(bufr, "%i", (BH_1CPO.pitch() / 100));
      break;

    case CharSpeed:
      sprintf(bufr, "%i", BH_1CPO.userCharSpeed());
      break;
  }
  BH_1CPO.sendMessage(bufr);
}

void sendSettingModeAndValue()
{
  // send setting mode and value:
    
  sendSettingMode();
  sendSettingValue();
}

BHMorse_Wpm nextSpeed(BHMorse_Wpm pSpeed)
{
  // return the next speed in the array of discrete speeds
  // that is greater than the passed speed

  byte p = 0;
  byte numVals = BH_ARRAY_SIZE(BH_1CPO_Speeds);

  // get the index in the array of the biggest element <= the passed value,
  // or 0 if < the lower item:
  
  for (byte i = 0; i < numVals; i++)
  {
    if (BH_1CPO_Speeds[i] <= pSpeed) p = i;
  }

  // bump and wrap:

  p++; if (p >= numVals) p = 0;

  return BH_1CPO_Speeds[p];
}

BHMorse_Hz nextPitch(BHMorse_Hz pPitch)
{
  // return the next pitch in the array of discrete pitches
  // that is greater than the passed pitch

  byte p = 0;
  byte numVals = BH_ARRAY_SIZE(BH_1CPO_Pitches);

  // get the index in the array of the biggest element <= the passed value,
  // or 0 if < the lower item:
  
  for (byte i = 0; i < numVals; i++)
  {
    if (BH_1CPO_Pitches[i] <= pPitch) p = i;
  }

  // bump and wrap:

  p++; if (p >= numVals) p = 0;

  return BH_1CPO_Pitches[p];
}


BHMorse_charElemMap_Group nextDifficulty(BHMorse_charElemMap_Group pDifficulty)
{
  // return the next difficulty in the array of discrete difficulties
  // that is greater than the passed difficulty

  byte p = 0;
  byte numVals = BH_ARRAY_SIZE(BH_1CPO_Difficulties);

  // get the index in the array of the biggest element <= the passed value,
  // or 0 if < the lower item:
  
  for (byte i = 0; i < numVals; i++)
  {
    if (BH_1CPO_Difficulties[i] <= pDifficulty) p = i;
  }

  // bump and wrap:

  p++; if (p >= numVals) p = 0;

  return BH_1CPO_Difficulties[p];
}

void rotateSettingMode()
{
  // rotate setting mode

  /// CUSTOMIZE: you can change this order, as long as it makes a circle
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
  
  /// CUSTOMIZE: you can change this order, as long as it makes a circle
  switch (sendingWhat)
  {
    case BHMorse::SendQSOs:
      sendingWhat = BHMorse::SendGroups;
      break;
      
    case BHMorse::SendGroups:
      sendingWhat = BHMorse::SendChars;
      break;

    case BHMorse::SendChars:
      sendingWhat = BHMorse::SendQSOs;
      break;

    default: // shouldn't be here
    case BHMorse::Idle:
      sendingWhat = BHMorse::SendQSOs;
      break;
  }
}

void rotateOverallSpeed()
{
  // rotate Overall Speed:

  BH_1CPO.setOverallSpeed(nextSpeed(BH_1CPO.overallSpeed()));
}

void rotatePitch()
{
  // rotate pitch:
  
  BH_1CPO.setPitch(nextPitch(BH_1CPO.pitch()), true);
}

void rotateCharacterSpeed()
{
  // rotate Character Speed:  

  BH_1CPO.setUserCharSpeed(nextSpeed(BH_1CPO.userCharSpeed()));
}

void rotateDifficulty()
{
  // rotate difficulty:
  
  BH_1CPO.setHighestEnabledGroup(nextDifficulty(BH_1CPO.highestEnabledGroup()));
}

void rotateSettingValue()
{
  // rotate setting value of current setting mode

  /// CUSTOMIZE: you can change this order, as long as it makes a circle
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
  waitForPulledupButtonRelease(BH_1CPO_PB1);

  sending = !sending;
  
  if (sending)
  {
    BH_1CPO.setRunMode(sendingWhat);
  }
  else
  {
    BH_1CPO.setRunMode(BHMorse::Idle);
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
  waitForPulledupButtonRelease(BH_1CPO_PB2);

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
  waitForPulledupButtonRelease(BH_1CPO_PB3);

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
