#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_OxygenSensor.h>

#define O2_ADDR      ADDRESS_3
#define COLLECT_NUM  10   // muestras internas que promedia la librería

DFRobot_OxygenSensor o2;   // usa &Wire por defecto

// La librería no comprueba el resultado de requestFrom(): si la lectura I2C
// falla, rxbuf queda en ceros y ese 0.0 entra al promedio como si fuera una
// medición válida (DFRobot_OxygenSensor.cpp:94-115). El sensor deja de atender
// el bus mientras escribe su calibración en flash — de ahí el Error 263 al
// pulsar el botón de calibrate. Comprobamos aquí antes de fiarnos del valor.
static bool sensorResponde() {
    Wire.beginTransmission(O2_ADDR);
    return Wire.endTransmission() == 0;
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== test_o2_sen0322 ===");

    while (!o2.begin(O2_ADDR)) {
        Serial.println("ERROR: SEN0322 no detectado en el bus I2C. Reintentando...");
        delay(1000);
    }
    Serial.println("SEN0322 detectado. Iniciando lecturas (warm-up esperado: 3-10 min)...");
}

void loop() {
    if (!sensorResponde()) {
        // No llamamos a getOxygenData(): evitamos que un cero entre al promedio.
        Serial.printf("t=%lus  SKIP: el sensor no responde (ocupado o desconectado)\n",
                      millis() / 1000);
        delay(1000);
        return;
    }

    float o2pct = o2.getOxygenData(COLLECT_NUM);

    if (o2pct <= 0.0f) {
        Serial.printf("t=%lus  DESCARTADA: lectura invalida (%.2f)\n",
                      millis() / 1000, o2pct);
    } else {
        Serial.printf("t=%lus  O2: %.2f %%Vol\n", millis() / 1000, o2pct);
    }

    delay(1000);
}
