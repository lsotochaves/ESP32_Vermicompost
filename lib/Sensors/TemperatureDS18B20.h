#pragma once
#include "core/ISensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

namespace vermi {

class TemperatureDS18B20 : public ISensor {
public:
    static constexpr uint8_t BIT_RESOLUTION = 12;

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