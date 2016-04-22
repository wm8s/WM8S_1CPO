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
//  Helper Procedures:
//
////////////////////////////////////////////////////////////////////////////////

/// TODO: MOVE TO BH ///
void debouncePB()
{
  // delay long enough for the switch to debounce:
  
  delay(BH_1CPO_DEBOUNCE_TIME);
}

/// TODO: MOVE TO BH ///
void waitForPulledupButtonRelease(byte pPin)
{
  // wait for him to let up on the switch:
  
  debouncePB();
  while (isPulledupButtonPressed(pPin)) {};
}

/// TODO: MOVE TO BH ///
bool isPulledupButtonPressed(byte pPin)
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
          bufr[0] = 'Q';
          break;

        case BHMorse::SendGroups:
          bufr[0] = 'G';
          break;

        case BHMorse::SendChars:
          bufr[0] = 'C';
          break;

        default:    // huh?
          bufr[0] = '?';
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
  BH_1CPO.sendMessage(" ");
  sendSettingValue();
}

BHMorse_Wpm nextSpeed(BHMorse_Wpm pSpeed)
{
  // return the next speed in the array of discrete speeds
  // that is greater than the passed speed

  byte p = 0;
  byte numVals = ARRAY_SIZE(BH_1CPO_Speeds);

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
  byte numVals = ARRAY_SIZE(BH_1CPO_Pitches);

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
  byte numVals = ARRAY_SIZE(BH_1CPO_Difficulties);

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
  
  BH_1CPO.beepHi();
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
  
  BH_1CPO.beepHi();
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
  
  BH_1CPO.beepHi();
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
