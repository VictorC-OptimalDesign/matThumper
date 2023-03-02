// === GLOBAL CONSTANTS ========================================================

static int const Left = 3;
static int const Right = 4;

static int const Off = HIGH;
static int const On = LOW;

static int const DisableSolenoidDelayMS = 100;


// === GLOBAL VARIABLES ========================================================

static bool leftEnabled = false;
static bool rightEnabled = false;
static programStatus_t programStatus =
{
  .enabled = false,
  .step = 0u,
  .alarm =
  {
    .startTimeMS = 0u,
    .durationMS = 0u,
    .armed = false,    
  },
};
static program_t const* activeProgram = NULL;


// === PRIVATE FUNCTIONS =======================================================

static resetProgramStatus(void)
{
  programStatus.enabled = false;
  programStatus.step = 0u;
  programStatus.alarm =
  {
    .startTimeMS = 0u,
    .durationMS = 0u,
    .armed = false,
  };
}

static void updateSolenoid(mode_t mode)
{
  switch (mode)
  {
    case mode_leftOn:
      solenoid_enableLeft(true);
      break;

    case mode_leftOff:
      solenoid_enableLeft(false);
      break;

    case mode_rightOn:
      solenoid_enableRight(true);
      break;

    case mode_rightOff:
      solenoid_enableRight(false);
      break;

    default:
      solenoid_enableLeft(false);
      solenoid_enableRight(false);
  }
}


// === PUBLIC FUNCTIONS ========================================================

void solenoid_printStatus(void)
{
  static char const* LeftString = "<<  ";
  static char const* OffString = " -- ";
  static char const* RightString = "  >>";
  static char const* InvalidString = "????";

  char const* stateString = OffString;
  if (leftEnabled)
  {
    if (rightEnabled)
      stateString = InvalidString;
    else
      stateString = LeftString;
  }
  else if (rightEnabled)
    stateString = RightString;

  printf("%lu,%s,left=%u,right=%u,",
    millis(),
    stateString,
    leftEnabled, rightEnabled);
  if ((activeProgram != NULL) && (programStatus.enabled))
  {
    static char const* ModeString[] =
    {
      "reset",
      "left on",
      "left off",
      "right on",
      "right off",
    };
    printf("%s,", ModeString[activeProgram->steps[programStatus.step].mode]);
  }
  printf("\n");
}

void solenoid_enableLeft(bool enable)
{
  if (enable != leftEnabled)
  {
    if (enable)
    {
      if (rightEnabled)
      {
        solenoid_enableRight(false);
        delay(DisableSolenoidDelayMS);
      }
    }
    leftEnabled = enable;
    digitalWrite(Left, (leftEnabled) ? (On) : (Off));
  }
}

void solenoid_enableRight(bool enable)
{
  if (enable != rightEnabled)
  {
    if (enable)
    {
      if (leftEnabled)
      {
        solenoid_enableLeft(false);
        delay(DisableSolenoidDelayMS);
      }
    }
    rightEnabled = enable;
    digitalWrite(Right, (rightEnabled) ? (On) : (Off));
  }
}

void solenoid_init(void)
{
  pinMode(Left, OUTPUT);
  pinMode(Right, OUTPUT);

  digitalWrite(Left, Off);
  leftEnabled = false;

  digitalWrite(Right, Off);
  rightEnabled = false;
}

void solenoid_startProgram(program_t const* program)
{
  if ((program != NULL) && (program->steps != NULL) && (!programStatus.enabled))
  {
    activeProgram = program;
    resetProgramStatus();
    programStatus.enabled = true;
    uint32_t durationMS = activeProgram->steps[programStatus.step].durationMS;
    alarm_arm(&programStatus.alarm, durationMS);
  }
}

void solenoid_stopProgram(void)
{
  if (programStatus.enabled)
  {
    resetProgramStatus();
    activeProgram = NULL;
    solenoid_enableLeft(false);
    solenoid_enableRight(false);
  }
}

void solenoid_process(void)
{
  if ((activeProgram != NULL) && (programStatus.enabled))
  {
    if (alarm_hasElapsed(&programStatus.alarm))
    {
      
      ++programStatus.step;
      if (programStatus.step >= activeProgram->numberOfSteps)
        programStatus.step = 0u;

      updateSolenoid(activeProgram->steps[programStatus.step].mode);

      uint32_t durationMS = activeProgram->steps[programStatus.step].durationMS;
      alarm_arm(&programStatus.alarm, durationMS);
      solenoid_printStatus();
    }
  }
}
