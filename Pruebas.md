# Pruebas de Hardware — Vermicompost Monitor

## DS18B20 — Sensor de Temperatura

### Prueba de temperatura con la mano
- **Descripción:** Se sostuvo el sensor con la mano para transferirle calor por contacto directo.
- **Resultado:** La temperatura subió gradualmente durante el contacto. Al soltar el sensor, la temperatura bajó lentamente por convección con el aire.
- **Observación:** El sensor tiene inercia térmica notable — comportamiento esperado y favorable para medir la masa de un lecho de vermicompost (proceso lento).
- **Estado:** ✅ Exitosa

---

### Prueba de desconexión y reconexión
- **Descripción:** Se desconectó el pin DQ del sensor mientras el sistema estaba en operación.
- **Resultado:** El sistema reportó `ERROR: sensor desconectado` correctamente. Al reconectar el DQ, las lecturas se reanudaron sin necesidad de reiniciar el ESP32.
- **Estado:** ✅ Exitosa

---

## Deep Sleep + Temperatura

### Prueba de ciclo deep sleep con lectura y envío por UART
- **Archivo:** `hardware_tests/test_deepSlp_temp.cpp`
- **Descripción:** El ESP32 despierta por timer, lee el DS18B20 en el pin D4 (GPIO4), envía la temperatura por UART y entra en deep sleep por 10 segundos. Cada despertar re-ejecuta `setup()` desde cero.
- **Resultado:** El chip reportó `rst:0x5 (DEEPSLEEP_RESET)` confirmando el despertar desde deep sleep, seguido de una lectura correcta (`24.31 °C`) y la entrada al siguiente ciclo de sleep.
- **Observación:** Valida la base del sistema — despertar por tiempo, captura de datos y transmisión — sin corromper el ciclo entre reinicios.
- **Estado:** ✅ Exitosa
