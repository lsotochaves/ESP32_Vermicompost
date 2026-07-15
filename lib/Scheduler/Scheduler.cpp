#include "Scheduler.h"
#include <esp_attr.h>
#include <algorithm>

namespace vermi {

// --- State that must survive deep sleep -> RTC memory ---
// Survives deep sleep, but NOT a power loss / hard reset (that's RTC memory's nature).
RTC_DATA_ATTR static uint32_t s_clockSec         = 0; // our monotonic notion of time
RTC_DATA_ATTR static uint32_t s_lastSampleSec    = 0;
RTC_DATA_ATTR static uint32_t s_lastSendSec      = 0;
RTC_DATA_ATTR static uint32_t s_lastPlannedSleep = 0;
RTC_DATA_ATTR static bool     s_firstBoot        = true;

Scheduler::Scheduler(const ScheduleConfig& cfg) : _cfg(cfg) {}

WakeAction Scheduler::onWake() {
    if (s_firstBoot) {
        s_firstBoot     = false;
        s_clockSec      = 0;
        s_lastSampleSec = 0;
        s_lastSendSec   = 0;
    } else {
        // We don't have a wall clock across deep sleep, so we advance our
        // monotonic counter by the sleep we requested last cycle. Good enough
        // for a slow process; sync real time via NTP at send time if needed.
        s_clockSec += s_lastPlannedSleep;
    }

    s_lastSampleSec = s_clockSec;  // every scheduled wake is a sample wake

    if (s_clockSec - s_lastSendSec >= _cfg.sendIntervalSec) {
        s_lastSendSec = s_clockSec;
        return WakeAction::SampleAndSend;
    }
    return WakeAction::SampleOnly;
}

uint32_t Scheduler::secondsUntilNextWake() {
    const uint32_t nextSample = s_lastSampleSec + _cfg.sampleIntervalSec;
    const uint32_t nextSend   = s_lastSendSec   + _cfg.sendIntervalSec;
    const uint32_t next       = std::min(nextSample, nextSend);
    uint32_t sleep = (next > s_clockSec) ? (next - s_clockSec)
                                         : _cfg.sampleIntervalSec;
    s_lastPlannedSleep = sleep;
    return sleep;
}

uint32_t Scheduler::nowSec() const { return s_clockSec; }

} // namespace vermi
