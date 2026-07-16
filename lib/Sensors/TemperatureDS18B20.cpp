#include "TemperatureDS18B20.h"

namespace vermi {

bool TemperatureDS18B20::begin() {
    _tempSensor.begin();
    _tempSensor.setResolution(BIT_RESOLUTION);
    return _tempSensor.getDeviceCount() > 0;
}

size_t TemperatureDS18B20::sample(Reading* out, size_t maxOut, uint32_t now) {
    if (maxOut < 1) return 0;

    _tempSensor.requestTemperatures();
    float celsius = _tempSensor.getTempCByIndex(0);

    if (celsius == DEVICE_DISCONNECTED_C) return 0;

    out[0] = { _id, METRIC_TEMPERATURE_C, celsius, now };
    return 1;
}

} // namespace vermi