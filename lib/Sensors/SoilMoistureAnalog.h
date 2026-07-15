#pragma once
#include "core/ISensor.h"

namespace vermi {

// Concrete example: a capacitive soil-moisture probe read via the ADC.
// Demonstrates the ANALOG case -> it does NOT use an IBus, it reads the pin
// directly. Use an ADC1 pin (GPIO 32-39) so it keeps working with WiFi on.
//
// Calibration: read the raw ADC value in air (dryRaw) and fully wet (wetRaw)
// once, hard-code them here. The class maps raw -> 0..100 %.
class SoilMoistureAnalog : public ISensor {
public:
    SoilMoistureAnalog(uint16_t id, int adcPin, int dryRaw, int wetRaw);

    bool   begin() override;
    size_t sample(Reading* out, size_t maxOut, uint32_t now) override;
    const char* name() const override { return "SoilMoistureAnalog"; }
    uint16_t    id()   const override { return _id; }

private:
    uint16_t _id;
    int _pin, _dry, _wet;
};

} // namespace vermi
