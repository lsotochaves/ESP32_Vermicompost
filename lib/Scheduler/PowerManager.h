#pragma once
#include <cstdint>

namespace vermi {

// Thin wrapper over the ESP32 deep-sleep machinery. Kept separate from
// Scheduler so the *decision* (when) and the *mechanism* (how to sleep)
// are independent and individually replaceable/testable.
class PowerManager {
public:
    // Arm the RTC timer and enter deep sleep. DOES NOT RETURN:
    // on wake the chip reboots straight into setup().
    static void deepSleepFor(uint32_t seconds);

    // Optional: also wake on a GPIO edge -- e.g. an alert line from a
    // threshold sensor, or a lid switch. A hook for future "alarm" behaviour.
    static void enableExternalWake(int gpio, int level);
};

} // namespace vermi
