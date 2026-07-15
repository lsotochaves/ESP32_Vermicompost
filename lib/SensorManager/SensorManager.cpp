#include "SensorManager.h"

namespace vermi {

bool SensorManager::add(ISensor* s) {
    if (_n >= MAX_SENSORS || s == nullptr) return false;
    _sensors[_n++] = s;
    return true;
}

void SensorManager::beginAll() {
    for (size_t i = 0; i < _n; ++i) {
        _sensors[i]->begin();   // a false return could be logged/flagged here
    }
}

size_t SensorManager::sampleAll(Reading* out, size_t maxOut, uint32_t now) {
    size_t total = 0;
    for (size_t i = 0; i < _n && total < maxOut; ++i) {
        _sensors[i]->wake();
        total += _sensors[i]->sample(out + total, maxOut - total, now);
        _sensors[i]->sleep();
    }
    return total;
}

} // namespace vermi
