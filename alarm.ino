// === MACROS ==================================================================

#define GET_TIME_MS()                   millis()


// === PUBLIC FUNCTIONS ========================================================

void alarm_arm(alarm_t volatile* alarm, uint32_t durationMS)
{
    if (alarm != NULL)
    {
        alarm->durationMS = durationMS;
        alarm->startTimeMS = (uint32_t)GET_TIME_MS();
        alarm->armed = true;
    }
}


void alarm_disarm(alarm_t volatile* alarm)
{
    if (alarm != NULL)
    {
        alarm->armed = false;
        alarm->durationMS = 0;
    }
}


void alarm_snooze(alarm_t volatile* alarm, uint32_t additionalTimeMS)
{
    if ((alarm != NULL) && alarm->armed)
    {
        if ((UINT32_MAX - additionalTimeMS) >= alarm->durationMS)
            alarm->durationMS += additionalTimeMS;
        else
            alarm->durationMS = UINT32_MAX;
    }
}


bool alarm_hasElapsed(alarm_t volatile* alarm)
{
    // Flag indicating if the alarm has elapsed; initialized to false (has not
    // fired yet) and the subsequent code will determine if it has fired (true).
    bool elapsed = false;
    if ((alarm != NULL) && alarm->armed)
    {
        if ((alarm->durationMS == 0 ) || (GET_TIME_MS() - alarm->startTimeMS >= alarm->durationMS))
        {
            alarm->durationMS = 0;
            elapsed = true;
        }
    }
    return elapsed;
}
