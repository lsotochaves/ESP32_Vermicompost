#include "WiFiMqttTransport.h"
#include <Arduino.h>
// #include <WiFi.h>
// #include <PubSubClient.h>
// #include "secrets.h"   // WIFI_SSID, WIFI_PASS, MQTT_HOST, ...

namespace vermi {

bool WiFiMqttTransport::connect() {
    // WiFi.begin(WIFI_SSID, WIFI_PASS);
    // wait (with timeout) for WL_CONNECTED, then connect MQTT client.
    return true; // placeholder
}

bool WiFiMqttTransport::send(const Reading* readings, size_t count) {
    // Serialize each Reading to JSON/binary and publish to a topic such as
    // "vermicompost/<deviceId>/readings". Batch into one payload if possible.
    for (size_t i = 0; i < count; ++i) {
        // mqtt.publish(topic, serialize(readings[i]));
        (void)readings[i];
    }
    return true; // placeholder
}

void WiFiMqttTransport::disconnect() {
    // mqtt.disconnect(); WiFi.disconnect(true); WiFi.mode(WIFI_OFF);
}

} // namespace vermi
