#include "TemperatureDS18B20.h"
// #include <OneWire.h>
// #include <DallasTemperature.h>

namespace vermi {

// TODO: instantiate OneWire(_pin) + DallasTemperature, begin the bus.
bool TemperatureDS18B20::begin() {
    // ds.begin(); return ds.getDeviceCount() > 0;
    return true; // placeholder
}

// TODO: ds.requestTemperatures(); float c = ds.getTempCByIndex(0);
size_t TemperatureDS18B20::sample(Reading* out, size_t maxOut, uint32_t now) {
    if (maxOut < 1) return 0;
    float celsius = 22.0f; // placeholder -> replace with real read
    out[0] = { _id, METRIC_TEMPERATURE_C, celsius, now };
    return 1;
}

} // namespace vermi
