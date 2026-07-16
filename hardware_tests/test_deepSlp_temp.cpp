#include <Arduino.h>
#include "TemperatureDS18B20.h"
#include "PowerManager.h"
#include "core/Reading.h"

using namespace vermi;

static constexpr int      SENSOR_PIN = 4;
static constexpr uint32_t SLEEP_SEC  = 10;

void setup() {
    Serial.begin(115200);
    delay(50);

    TemperatureDS18B20 sensor(METRIC_TEMPERATURE_C, SENSOR_PIN);

    if (!sensor.begin()) {
        Serial.println("[vermi] Sensor no encontrado en D4");
    } else {
        Reading result[1];
        size_t count = sensor.sample(result, 1, 0);

        if (count > 0) {
            Serial.printf("[vermi] Temperatura: %.2f C\n", result[0].value);
        } else {
            Serial.println("[vermi] Error al leer el sensor");
        }
    }

    Serial.printf("[vermi] Entrando en deep sleep por %u segundos...\n", SLEEP_SEC);
    Serial.flush();

    PowerManager::deepSleepFor(SLEEP_SEC);
}

void loop() {}
