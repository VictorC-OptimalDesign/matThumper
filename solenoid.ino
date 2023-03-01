// === GLOBAL CONSTANTS ========================================================

static int const Left = 3;
static int const Right = 4;

static int const Off = HIGH;
static int const On = LOW;

static int const DisableSolenoidDelayMS = 100;


// === GLOBAL VARIABLES ========================================================

static bool leftEnabled = false;
static bool rightEnabled = false;

// === FUNCTIONS ===============================================================

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

  printf("%lu,%s,left=%u,right=%u,\n",
    millis(),
    stateString,
    leftEnabled, rightEnabled);
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
