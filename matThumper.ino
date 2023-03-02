// === INCLUDES ================================================================

#include <stdbool.h>
#include <stdint.h>

#include "userTypes.h"


// === GLOBAL CONSTANTS ========================================================

/// Version string.
static char* const Version = "0.0.2";


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
        static step_t const steps[] =
        {
          {
            .mode = mode_reset,
            .durationMS = 13000,
          },
          {
            .mode = mode_leftOn,
            .durationMS = 1000,
          },
          {
            .mode = mode_leftOff,
            .durationMS = 14000,
          },
          {
            .mode = mode_rightOn,
            .durationMS = 1000,
          },
          {
            .mode = mode_rightOff,
            .durationMS = 1000,
          },
        };
        static program_t const program =
        {
          .steps = steps,
          .numberOfSteps = sizeof(steps) / sizeof(step_t),
        };
        solenoid_startProgram(&program);
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

  solenoid_init();
  solenoid_printStatus();
}

void loop()
{
  processInput();
  solenoid_process();
}
