#pragma once
#include <cstdint>

namespace vermi {

// Two independent cadences: how often we MEASURE, and how often we SEND.
struct ScheduleConfig {
    uint32_t sampleIntervalSec;   // e.g. 900   -> sample every 15 min
    uint32_t sendIntervalSec;     // e.g. 21600 -> send  every  6 h
};

enum class WakeAction {
    SampleOnly,      // just read the sensors and buffer the result
    SampleAndSend,   // read, buffer, AND flush the buffer over the transport
};

// Decides, on each wake, what to do and how long to sleep next.
// All timing state lives in RTC memory (see Scheduler.cpp) so it
// survives deep sleep. The Scheduler does NOT touch sensors or radios;
// it only produces decisions. That keeps orchestration testable.
class Scheduler {
public:
    explicit Scheduler(const ScheduleConfig& cfg);

    WakeAction onWake();               // advance the clock, decide this cycle's action
    uint32_t   secondsUntilNextWake(); // compute next event and remember it
    uint32_t   nowSec() const;         // current monotonic time, for stamping readings

private:
    ScheduleConfig _cfg;
};

} // namespace vermi
