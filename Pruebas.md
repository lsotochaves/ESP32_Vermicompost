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
