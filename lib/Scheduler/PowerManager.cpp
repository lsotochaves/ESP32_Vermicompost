#include "PowerManager.h"
#include <esp_sleep.h>
#include <driver/gpio.h>

namespace vermi {

void PowerManager::deepSleepFor(uint32_t seconds) {
    esp_sleep_enable_timer_wakeup(static_cast<uint64_t>(seconds) * 1000000ULL);
    esp_deep_sleep_start();   // never returns
}

void PowerManager::enableExternalWake(int gpio, int level) {
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(gpio), level);
}

} // namespace vermi
