#include <BHMorse.h>

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
//  You can [probably] safely change anything marked with /// CUSTOMIZE: tag.
//  Anything, and you're on your own.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BH1CPO_H__
#define __BH1CPO_H__

////////////////////////////////////////////////////////////////////////////////
//
//  Defines, enums, and typedefs, oh my
//
////////////////////////////////////////////////////////////////////////////////

#define BH_1CPO_TONEPIN A0										/// CUSTOMIZE
#define BH_1CPO_PB1 A1												/// CUSTOMIZE
#define BH_1CPO_PB2 A4												/// CUSTOMIZE
#define BH_1CPO_PB3 A5												/// CUSTOMIZE

#define BH_1CPO_DEBOUNCE_TIME 10							/// CUSTOMIZE

#define BH_1CPO_DEFAULT_SETTING_MODE SendWhat

// tones

#define BH_1CPO_TONE_HI_1_LOOPS 3							/// CUSTOMIZE
#define BH_1CPO_TONE_HI_1_HZ	1000						/// CUSTOMIZE
#define BH_1CPO_TONE_HI_1_DURATION 25					/// CUSTOMIZE
#define BH_1CPO_TONE_HI_2_HZ 2000							/// CUSTOMIZE
#define BH_1CPO_TONE_HI_2_DURATION 25					/// CUSTOMIZE
#define BH_1CPO_TONE_HI_3_DURATION 100				/// CUSTOMIZE

#define BH_1CPO_PITCHOFFSET_SETTINGMODE 50		/// CUSTOMIZE

// max size of setting conformation for now (1st letter):

#define BH_1CPO_MAXSETTINGCFMBUFRSIZE 2

// max size of setting value confirmation for now ("r", "oo", "d", "pp"00, "cc"):

#define BH_1CPO_MAXSETTINGVALUECFMBUFRSIZE 3

// Setting modes;

enum BH_1CPO_Setting_Mode											/// CUSTOMIZE (nationalize)
{
  SendWhat = 'S',
  OverallSpeed = 'O',
  Difficulty = 'D',
  Pitch = 'P',
  CharSpeed = 'C'
};

enum BH_1CPO_SendWhat_Mode										/// CUSTOMIZE (nationalize)
{
  SendQSOs = 'Q',
  SendGroups = 'G',
  SendChars = 'C',
  Unknown = '?'
};

// Discrete speeds to rotate through;

const byte BH_1CPO_Speeds[] =									/// CUSTOMIZE
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

const unsigned int BH_1CPO_Pitches[] =				/// CUSTOMIZE
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

const unsigned int BH_1CPO_Difficulties[] =
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

void initIO();
void handleReset();
void beep();
void startMorse();

void debouncePB();
void waitForPulledupButtonRelease(BH_PIN pPin);
bool isPulledupButtonPressed(BH_PIN pPin);

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

