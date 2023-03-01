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
