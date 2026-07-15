#pragma once
#include <cstddef>
#include "core/Reading.h"

namespace vermi {

// The bridge between sampling and sending. Accumulates readings across many
// deep-sleep cycles by living in RTC memory, then is flushed on a send cycle.
//
// IMPORTANT: capacity is fixed at compile time because RTC "slow" memory is
// tiny (~8 KB, shared with other RTC_DATA_ATTR state). With sizeof(Reading)=12
// bytes, 256 readings ~= 3 KB. Tune VERMI_RTC_BUFFER_CAPACITY to your budget.
class RtcDataStore {
public:
    bool           append(const Reading& r); // false if the buffer is full
    size_t         count() const;
    const Reading* data()  const;            // pointer to the contiguous buffer
    void           clear();                  // call after a successful send
    bool           isFull() const;
};

} // namespace vermi
