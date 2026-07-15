#include "SoilMoistureAnalog.h"
#include <Arduino.h>

namespace vermi {

SoilMoistureAnalog::SoilMoistureAnalog(uint16_t id, int adcPin, int dryRaw, int wetRaw)
    : _id(id), _pin(adcPin), _dry(dryRaw), _wet(wetRaw) {}

bool SoilMoistureAnalog::begin() {
    pinMode(_pin, INPUT);
    analogReadResolution(12);   // 0..4095
    return true;
}

size_t SoilMoistureAnalog::sample(Reading* out, size_t maxOut, uint32_t now) {
    if (maxOut < 1) return 0;

    const int raw = analogRead(_pin);
    // Map raw -> percent. dry = 0 %, wet = 100 %. Clamp to [0,100].
    float pct = 100.0f * (float)(_dry - raw) / (float)(_dry - _wet);
    if (pct < 0)   pct = 0;
    if (pct > 100) pct = 100;

    out[0] = { _id, METRIC_SOIL_MOIST_PCT, pct, now };
    return 1;
}

} // namespace vermi
