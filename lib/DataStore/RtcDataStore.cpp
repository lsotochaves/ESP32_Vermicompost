#include "RtcDataStore.h"
#include <esp_attr.h>

namespace vermi {

#ifndef VERMI_RTC_BUFFER_CAPACITY
#define VERMI_RTC_BUFFER_CAPACITY 256
#endif

// These survive deep sleep. They do NOT survive a power loss or hard reset.
// If your data is mission-critical, mirror to NVS/LittleFS (flash) instead.
RTC_DATA_ATTR static Reading s_buffer[VERMI_RTC_BUFFER_CAPACITY];
RTC_DATA_ATTR static size_t  s_count = 0;

bool RtcDataStore::append(const Reading& r) {
    if (s_count >= VERMI_RTC_BUFFER_CAPACITY) return false;
    s_buffer[s_count++] = r;
    return true;
}
size_t         RtcDataStore::count() const { return s_count; }
const Reading* RtcDataStore::data()  const { return s_buffer; }
void           RtcDataStore::clear()       { s_count = 0; }
bool           RtcDataStore::isFull() const { return s_count >= VERMI_RTC_BUFFER_CAPACITY; }

} // namespace vermi
