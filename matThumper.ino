// === INCLUDES ================================================================

#include <stdbool.h>
#include <stdint.h>

#include "userTypes.h"


// === DEFINES =================================================================

// Duration for off between shots.
#define BETWEEN_SHOT_LOWER_MS           (12000u)
#define BETWEEN_SHOT_UPPER_MS           (20000u)

// Duration for off when gathering arrows.
#define OFF_GATHER_LOWER_MS             (60000u)
#define OFF_GATHER_UPPER_MS             (75000u)

// Duration for left/right thump (solenoid on).
#define ON_LOWER_MS                     (250u)
#define ON_UPPER_MS                     (250u)

// Duration for when the solenoid is off.
#define OFF_SHOT_LOWER_MS               (BETWEEN_SHOT_LOWER_MS - ON_LOWER_MS)
#define OFF_SHOT_UPPER_MS               (BETWEEN_SHOT_UPPER_MS - ON_UPPER_MS)


// === GLOBAL CONSTANTS ========================================================

// Version string.
static char* const Version = "0.0.11";

// Program 1
static step_t const Steps1[] =
{
  { mode_left, ON_LOWER_MS, ON_UPPER_MS },
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_LOWER_MS },
  { mode_right, ON_LOWER_MS, ON_UPPER_MS },
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_LOWER_MS },
};
static program_t const Program1 =
{
  .steps = Steps1,
  .numberOfSteps = sizeof(Steps1) / sizeof(step_t),
};

// Program 2
static step_t const Steps2[] =
{
  { mode_left, ON_LOWER_MS, ON_UPPER_MS },
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_UPPER_MS },
  { mode_right, ON_LOWER_MS, ON_UPPER_MS },
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_UPPER_MS },
};
static program_t const Program2 =
{
  .steps = Steps2,
  .numberOfSteps = sizeof(Steps2) / sizeof(step_t),
};

// Program 3
static step_t const Steps3[] =
{
  { mode_left, ON_LOWER_MS, ON_UPPER_MS },    // 1
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_UPPER_MS },
  { mode_right, ON_LOWER_MS, ON_UPPER_MS },   // 2
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_UPPER_MS },
  { mode_left, ON_LOWER_MS, ON_UPPER_MS },    // 3
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_UPPER_MS },
  { mode_right, ON_LOWER_MS, ON_UPPER_MS },   // 4
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_UPPER_MS },
  { mode_left, ON_LOWER_MS, ON_UPPER_MS },    // 5
  { mode_off, OFF_GATHER_LOWER_MS, OFF_GATHER_UPPER_MS },
  { mode_right, ON_LOWER_MS, ON_UPPER_MS },   // 1
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_UPPER_MS },
  { mode_left, ON_LOWER_MS, ON_UPPER_MS },    // 2
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_UPPER_MS },
  { mode_right, ON_LOWER_MS, ON_UPPER_MS },   // 3
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_UPPER_MS },
  { mode_left, ON_LOWER_MS, ON_UPPER_MS },    // 4
  { mode_off, OFF_SHOT_LOWER_MS, OFF_SHOT_UPPER_MS },
  { mode_right, ON_LOWER_MS, ON_UPPER_MS },   // 5
  { mode_off, OFF_GATHER_LOWER_MS, OFF_GATHER_UPPER_MS },
};
static program_t const Program3 =
{
  .steps = Steps3,
  .numberOfSteps = sizeof(Steps3) / sizeof(step_t),
};

// === GLOBAL VARIABLES ========================================================


// === PRINTF SUPPORT ==========================================================

// For printf support.
// See the following post:
// https://forum.arduino.cc/t/printf-on-arduino/888528/2
FILE f_out;
int sput(char c, __attribute__((unused)) FILE* f)
{
  return !Serial.write(c);
}


// === PRIVATE FUNCTIONS =======================================================

static void processInput(void)
{
  if (Serial.available())
  {
    int c = Serial.read();
    switch (c)
    {
      case '\n':
      {
        solenoid_printStatus();
      }
      break;
      
      case 'L':
      case 'l':
      {
        solenoid_stopProgram();
        solenoid_enableLeft(true);
        solenoid_printStatus();
      }
      break;

      case 'R':
      case 'r':
      {
        solenoid_stopProgram();
        solenoid_enableRight(true);
        solenoid_printStatus();
      }
      break;

      case 'N':
      case 'O':
      case 'n':
      case 'o':
      {
        solenoid_stopProgram();
        solenoid_enableLeft(false);
        solenoid_enableRight(false);
        solenoid_printStatus();
      }
      break;

      case '0':
      {
        solenoid_stopProgram();
        solenoid_printStatus();
      }
      break;

      case '1':
      {
        solenoid_startProgram(&Program1);
        solenoid_printStatus();
      }
      break;

      case '2':
      {
        solenoid_startProgram(&Program2);
        solenoid_printStatus();
      }
      break;

      case '3':
      {
        solenoid_startProgram(&Program3);
        solenoid_printStatus();
      }
      break;

      default:
      {
        // Do nothing for now.
      }

    }
  }
}

// === ARDUINO FUNCTIONS =======================================================

void setup()
{
  // put your setup code here, to run once:

  // Updated the baud rate based on the following reference to reduce the error;
  // this is based on a 16 MHz internal oscillator:
  // http://wormfood.net/avrbaudcalc.php
  Serial.begin(1000000);

  // cf https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gaf41f158c022cbb6203ccd87d27301226
  fdev_setup_stream(&f_out, sput, nullptr, _FDEV_SETUP_WRITE);
  stdout = &f_out;

  printf("\nMAT-101: Mathews Thumper (ver. %s)\n", Version);

  randomSeed(analogRead(0));

  solenoid_init();
  delay(100);
  solenoid_printHeader();
  delay(500);
  solenoid_printStatus();
}

void loop()
{
  processInput();
  solenoid_process();
}
