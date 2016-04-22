#include <BHMorse.h>

#ifndef __BH1CPO_H__
#define __BH1CPO_H__

////////////////////////////////////////////////////////////////////////////////
//
//  1CPO - My First Code Practice Oscillator. Like 3CPO, only mintier.
//
//  Written by: Rob Bailey, WM8S
//
////////////////////////////////////////////////////////////////////////////////
//
//  Header
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//  Defines, enums, and typedefs, oh my
//
////////////////////////////////////////////////////////////////////////////////

/// TODO: MOVE TO BH ///
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define BH_1CPO_TONEPIN 9
#define BH_1CPO_PB1 10
#define BH_1CPO_PB2 11
#define BH_1CPO_PB3 12

#define BH_1CPO_DEBOUNCE_TIME 10

#define BH_1CPO_DEFAULT_SETTING_MODE SendWhat

#define BH_1CPO_PITCHOFFSET_SETTINGMODE 50
#define BH_1CPO_PITCHOFFSET_SETTINGVALUE (-50)

// max size of setting conformation:

#define BH_1CPO_MAXSETTINGCFMBUFRSIZE 2

// max size of setting value confirmation for now ("r", "oo", "d", "pp"00, "cc"):

#define BH_1CPO_MAXSETTINGVALUECFMBUFRSIZE 3

// Setting modes;

enum BH_1CPO_Setting_Mode
{
  SendWhat = 'S',
  OverallSpeed = 'O',
  Difficulty = 'D',
  Pitch = 'P',
  CharSpeed = 'C'
};

// Discrete speeds to rotate through;

const byte BH_1CPO_Speeds[] =                 // this can be moved to PROGMEM if needed
{
  5,
  7,
  10,
  13,
  15,
  18,
  20,
  25,
  30,
  35,
  40,
  45,
  50
};

// Discrete pitches to rotate through:

const unsigned int BH_1CPO_Pitches[] =        // this can be moved to PROGMEM if needed
{
  400,
  500,
  600,
  700,
  800,
  900,
  1000,
  1100,
  1200,
  1300,
  1400,
  1500,
  1600,
  1700,
  1800
};

// Discrete difficulties to rotate through:

const unsigned int BH_1CPO_Difficulties[] =   // this can be moved to PROGMEM if needed
{
  1,
  2,
  3,
  4,
  5,
  6,
  7
};

////////////////////////////////////////////////////////////////////////////////
//
//  Procedures
//
////////////////////////////////////////////////////////////////////////////////

void debouncePB();                             /// TODO: MOVE TO BH ///
void waitForPulledupButtonRelease(byte pPin);  /// TODO: MOVE TO BH ///
bool isPulledupButtonPressed(byte pPin);       /// TODO: MOVE TO BH ///

void sendSettingMode();
void sendSettingValue();
void sendSettingModeAndValue();

BHMorse_Wpm nextSpeed(BHMorse_Wpm pSpeed);
BHMorse_Hz nextPitch(BHMorse_Hz pPitch);
BHMorse_charElemMap_Group nextDifficulty(BHMorse_charElemMap_Group pDifficulty);

void rotateSettingMode();
void rotateSendWhat();
void rotateOverallSpeed();
void rotatePitch();
void rotateCharacterSpeed();
void rotateDifficulty();
void rotateSettingValue();

void handlePB1Pressed();
void handlePB2Pressed();
void handlePB3Pressed();

////////////////////////////////////////////////////////////////////////////////
//
//  Global variables
//
////////////////////////////////////////////////////////////////////////////////

extern BHMorse BH_1CPO;
extern bool sending;
extern BHMorse::RunMode sendingWhat;
extern BH_1CPO_Setting_Mode settingMode;

#endif

