#pragma once
#include <cstddef>
#include "core/ISensor.h"
#include "core/Reading.h"

namespace vermi {

// Holds the set of sensors and treats them uniformly through ISensor.
// This is where "support any kind of sensor" actually happens: the manager
// never knows whether a sensor is I2C, analog or 1-Wire. Registering a new
// sensor (pH, CO2, ...) is one add() call in main; nothing else changes.
class SensorManager {
public:
    bool add(ISensor* s);                   // register a sensor instance
    void beginAll();                        // init all; failures are isolated

    // Wake -> sample -> sleep every sensor, writing into a transient buffer.
    // Returns total readings written. Deliberately does NOT touch storage,
    // so the manager stays decoupled from where data ends up.
    size_t sampleAll(Reading* out, size_t maxOut, uint32_t now);

    size_t count() const { return _n; }

private:
    static constexpr size_t MAX_SENSORS = 16;
    ISensor* _sensors[MAX_SENSORS] = { nullptr };
    size_t   _n = 0;
};

} // namespace vermi
