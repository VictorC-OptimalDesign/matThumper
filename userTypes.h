// === TYPE DEFINES: ALARMS ====================================================

typedef struct alarm_t
{
  /// The system time when the alarm was armed in milliseconds.
  uint32_t startTimeMS;

  /// The amount of time that needs to elapse before the alarm has "fired" in
  /// milliseconds.
  uint32_t durationMS;

  /// Flag indicating whether the alarm is armed.
  bool armed;

} alarm_t;


// === TYPE DEFINES: SOLENOID ==================================================

typedef enum mode_
{
  mode_off = 0,
  mode_left = 1,
  mode_right = 2,
} mode_t;

typedef struct step_t
{
  mode_t mode;
  uint32_t durationLowerMS;
  uint32_t durationUpperMS;
} step_t;

typedef struct program_t
{
  step_t const* steps;
  uint8_t numberOfSteps;
} program_t;

typedef struct programStatus_t
{
  bool enabled;
  uint8_t step;
  alarm_t alarm;
} programStatus_t;
