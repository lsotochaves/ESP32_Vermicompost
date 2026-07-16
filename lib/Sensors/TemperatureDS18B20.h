#pragma once
#include "core/ISensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

namespace vermi {

// Skeleton for a 1-Wire temperature probe (DS18B20) -- the classic choice for
// compost bed temperature (waterproof stainless versions exist). Shown to make
// the point: a DIFFERENT protocol, SAME ISensor contract. Fill in begin()/sample()
// using the OneWire + DallasTemperature libraries (add them to lib_deps).
class TemperatureDS18B20 : public ISensor {
public:
    TemperatureDS18B20(uint16_t id, int oneWirePin)
        : _id(id), _pin(oneWirePin), _oneWire(oneWirePin), _tempSensor(&_oneWire) {}

    bool   begin() override;
    size_t sample(Reading* out, size_t maxOut, uint32_t now) override;
    const char* name() const override { return "TemperatureDS18B20"; }
    uint16_t    id()   const override { return _id; }

private:
    uint16_t _id;
    int _pin;
    OneWire           _oneWire;
    DallasTemperature _tempSensor;
};

} // namespace vermi
