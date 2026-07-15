#pragma once
#include "core/ITransport.h"

namespace vermi {

// Skeleton transport: brings up WiFi, publishes the buffered readings over
// MQTT, then tears the radio down. Only ever called on a send cycle, so the
// expensive radio is off the rest of the time -> battery-friendly.
//
// Implementation note: use PubSubClient (or AsyncMqttClient) + WiFi.h.
// Put credentials/broker in a secrets header you do NOT commit.
class WiFiMqttTransport : public ITransport {
public:
    bool connect() override;
    bool send(const Reading* readings, size_t count) override;
    void disconnect() override;
};

} // namespace vermi
