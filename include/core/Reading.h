#pragma once
#include <cstdint>

namespace vermi {

// A single measurement of one physical quantity at one instant.
// Kept small and POD so it can be stored cheaply in RTC memory.
struct Reading {
    uint16_t sensorId;    // which sensor produced it (registry id)
    uint16_t metricId;    // what was measured (see MetricId)
    float    value;       // the value, in the metric's canonical unit
    uint32_t timestamp;   // monotonic seconds since first boot (see Scheduler::nowSec)
};

// Central catalogue of metrics so producers (sensors) and consumers
// (packers, backend) agree on meaning. Add future metrics here.
enum MetricId : uint16_t {
    METRIC_TEMPERATURE_C   = 1,   // bed temperature, °C
    METRIC_HUMIDITY_PCT    = 2,   // ambient relative humidity, %
    METRIC_SOIL_MOIST_PCT  = 3,   // substrate moisture, %
    METRIC_PH              = 4,   // substrate pH
    METRIC_CO2_PPM         = 5,   // CO2 concentration, ppm
    METRIC_NH3_PPM         = 6,   // ammonia, ppm (toxic to worms in excess)
    // ... future metrics
};

} // namespace vermi
