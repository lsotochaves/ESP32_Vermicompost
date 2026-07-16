#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_PIN 4

OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature tempSensor(&oneWire);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== test_temp_ds18b20 ===");

    tempSensor.begin();

    int found = tempSensor.getDeviceCount();
    Serial.printf("Sensores en el bus: %d\n", found);

    if (found == 0) {
        Serial.println("ERROR: ningún sensor detectado.");
        Serial.println("Revisa: pull-up 4.7k, cableado, GPIO correcto.");
    }
}

void loop() {
    tempSensor.requestTemperatures();
    float c = tempSensor.getTempCByIndex(0);

    if (c == DEVICE_DISCONNECTED_C) {
        Serial.println("ERROR: sensor desconectado");
    } else {
        Serial.printf("Temperatura: %.2f °C\n", c);
    }

    delay(1000);
}
