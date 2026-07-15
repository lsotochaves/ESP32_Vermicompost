#pragma once
#include <cstddef>
#include <cstdint>

namespace vermi {

// Abstracts the physical link to a digital device (I2C, SPI, ...).
// A sensor that talks over such a bus depends on THIS interface,
// never on a concrete bus implementation -> dependency injection.
//
// NOTE: not every protocol fits a register read/write model. Analog
// (ADC) and 1-Wire sensors typically access the hardware directly inside
// their own class and simply do not use an IBus. Don't force everything
// through this interface; use it where it genuinely helps (I2C/SPI).
class IBus {
public:
    virtual ~IBus() = default;
    virtual bool begin() = 0;
    virtual bool read (uint8_t reg, uint8_t* buf, size_t len) = 0;
    virtual bool write(uint8_t reg, const uint8_t* buf, size_t len) = 0;
};

} // namespace vermi
