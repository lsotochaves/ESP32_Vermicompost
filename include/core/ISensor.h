#pragma once
#include <cstddef>
#include "core/Reading.h"

namespace vermi {

// The single contract every sensor honours, regardless of protocol.
// A sensor knows ONLY how to initialise itself and produce readings.
// It does NOT know when it runs, how often, or where data goes.
// That separation is what lets the orchestrator stay generic.
class ISensor {
public:
    virtual ~ISensor() = default;

    // Called once per wake, before sampling. Return false on failure so
    // the manager can skip and flag this sensor without aborting the rest.
    virtual bool begin() = 0;

    // Optional low-power hooks. Many sensors need a warm-up after power-on;
    // the manager waits, the sensor just reports/handles readiness.
    virtual void wake()  {}
    virtual void sleep() {}

    // Take a measurement. Writes up to maxOut readings, returns how many.
    // One physical sensor MAY emit several metrics (e.g. temp + humidity).
    // 'now' is the monotonic timestamp to stamp the readings with.
    virtual size_t sample(Reading* out, size_t maxOut, uint32_t now) = 0;

    // Identity, for the registry and logging.
    virtual const char* name() const = 0;
    virtual uint16_t    id()   const = 0;
};

} // namespace vermi
