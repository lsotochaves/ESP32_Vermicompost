#pragma once
#include <cstddef>
#include "core/Reading.h"

namespace vermi {

// Abstracts how a batch of buffered readings leaves the device:
// WiFi+MQTT, WiFi+HTTP, LoRa, BLE... The orchestrator calls this ONLY
// during a send cycle, so the (power-hungry) radio is off the rest of the time.
class ITransport {
public:
    virtual ~ITransport() = default;
    virtual bool connect() = 0;                                  // bring the link up
    virtual bool send(const Reading* readings, size_t count) = 0;
    virtual void disconnect() = 0;                               // shut the radio down
};

} // namespace vermi
