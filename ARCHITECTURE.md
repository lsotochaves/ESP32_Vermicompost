# Vermicompost Monitor — Arquitectura de firmware (ESP32-D0WD-V3)

Primera arquitectura para monitoreo de una cama de vermicompost: el equipo
permanece en **deep sleep**, **muestrea** cada cierto intervalo y **envía** en
otro intervalo (independiente). Está diseñada para soportar *cualquier* tipo de
sensor (temperatura, humedad, pH, CO₂, NH₃, ...) sin tocar la orquestación.

## Idea central: deep sleep no es un `loop()`

Con deep sleep, cada despertar es un **reboot**: se ejecuta `setup()` de nuevo y
no sobrevive ningún objeto. Lo único persistente es la **RTC memory**
(`RTC_DATA_ATTR`, ~8 KB). De ahí salen tres decisiones de diseño:

1. El **Scheduler** no es un timer corriendo, sino lógica que en cada despertar
   decide la acción y cuánto dormir. Su reloj y marcas de tiempo viven en RTC.
2. El **buffer de lecturas** (`RtcDataStore`) vive en RTC memory, no en RAM
   normal (que se borra al dormir).
3. Los **sensores** son efímeros: se reconstruyen cada despertar, por eso no
   guardan estado de orquestación. Solo saben inicializarse y medir.

> Vermicompost es un proceso lento (cambios en horas), así que muestrear cada
> 15–30 min y dormir el resto es ideal para batería/solar.

## Capas y responsabilidades

| Capa            | Archivo                         | Sabe                                  | NO sabe                          |
|-----------------|---------------------------------|---------------------------------------|----------------------------------|
| Orquestación    | `src/main.cpp`                  | el flujo de un despertar              | detalles de cada protocolo       |
| Decisión        | `lib/Scheduler/Scheduler.*`     | cuándo muestrear / enviar             | cómo se duerme, qué es un sensor |
| Energía         | `lib/Scheduler/PowerManager.*`  | cómo entrar en deep sleep (timer/GPIO)| cuándo hacerlo                   |
| Registro        | `lib/SensorManager/*`           | muestrear a todos vía `ISensor`       | el protocolo concreto            |
| Sensor          | `lib/Sensors/*` + `ISensor`     | inicializarse y medir                 | cada cuánto, a dónde van datos   |
| Bus (opcional)  | `core/IBus.h`                   | leer/escribir un dispositivo digital  | qué significan los datos         |
| Almacenamiento  | `lib/DataStore/RtcDataStore.*`  | acumular lecturas entre sleeps        | cómo se envían                   |
| Transporte      | `lib/Transport/*` + `ITransport`| subir un lote (WiFi/MQTT/LoRa...)     | cuándo enviar, qué sensores hay  |

## Estructura de directorios

```
vermicompost-monitor/
├── platformio.ini              Configuración de build (board, flags, lib_deps)
├── include/core/               Interfaces COMPARTIDAS (el "contrato" del sistema)
│   ├── Reading.h               Struct de una medición + catálogo de métricas
│   ├── ISensor.h               Contrato único de todo sensor
│   ├── IBus.h                  Abstracción de bus digital (I2C/SPI), opcional
│   └── ITransport.h            Abstracción de envío
├── lib/                        Componentes modulares (cada carpeta = librería)
│   ├── Scheduler/              Scheduler (decisión) + PowerManager (deep sleep)
│   ├── DataStore/              Buffer en RTC memory
│   ├── SensorManager/          Registro + muestreo uniforme
│   ├── Sensors/                Drivers concretos (analógico, 1-Wire, ...)
│   └── Transport/              WiFi + MQTT (esqueleto)
├── src/main.cpp                Une todo: despierta → orquesta → duerme
├── hardware_tests/             Sketches de prueba con su propio setup()/loop()
├── test/                       Unit tests sin hardware (Scheduler es testeable)
└── PRUEBAS.md                  Bitácora de pruebas realizadas
```

> **hardware_tests/** guarda sketches que validan un componente contra hardware
> real (p. ej. `test_deepSlp_temp.cpp`). Cada uno trae su propio `setup()`/`loop()`,
> por eso vive **fuera** de `src/`: PlatformIO solo compila `src/` por defecto, así
> no colisiona con el `setup()` de `main.cpp`. Para correr uno, apuntá el build a
> ese archivo (`build_src_filter` / `src_dir` en `platformio.ini`). Los resultados
> se anotan en `PRUEBAS.md`.

`include/core` es el corazón reutilizable: **otros módulos del sistema general**
dependen de estas interfaces, no de implementaciones concretas.

## Flujo de un despertar

```
wake (timer RTC)
  → Scheduler.onWake()           ¿SampleOnly o SampleAndSend?
  → SensorManager.sampleAll()    wake → sample → sleep de cada ISensor
  → RtcDataStore.append(...)     persistir en RTC memory
  → si SampleAndSend:
        Transport.connect()      enciende WiFi
        Transport.send(buffer)   sube el lote
        store.clear()            si tuvo éxito
        Transport.disconnect()   apaga WiFi
  → PowerManager.deepSleepFor(n) duerme hasta el próximo evento (no retorna)
```

Leer y enviar nunca ocurren a la vez: el WiFi solo se enciende en el paso de
envío, lo que además evita el conflicto ADC2 + WiFi del ESP32.

## Cómo se extiende

- **Nuevo sensor** (p. ej. pH): crear `lib/Sensors/PhSensor.{h,cpp}` que herede
  de `ISensor`, añadir su métrica en `Reading.h`, y una línea `sensors.add(&ph)`
  en `main.cpp`. Nada más cambia.
- **Otro transporte** (LoRa): nueva clase que implemente `ITransport`.
- **Alerta por umbral**: usar `PowerManager::enableExternalWake()` para despertar
  por GPIO ante una condición crítica, no solo por timer.

## Notas de hardware (ESP32-D0WD-V3)

- Sensores analógicos: usar **ADC1** (GPIO 32–39) para que funcionen con WiFi.
- Evitar GPIO 6–11 (flash). Cuidado con strapping pins (0,2,4,5,12,15) al bootear.
- RTC slow memory ~8 KB: ajustar `VERMI_RTC_BUFFER_CAPACITY` según `sizeof(Reading)`.
- La RTC memory sobrevive el deep sleep, pero NO un corte de energía. Si los
  datos son críticos, persistir en flash (NVS/LittleFS) en vez de (o además de) RTC.
- Timestamps: aquí son monotónicos (segundos desde el primer boot). Para hora
  real, sincronizar por NTP en el ciclo de envío.

## Estado del esqueleto

Compilable como estructura. Tienen implementación real: `Scheduler`,
`PowerManager`, `RtcDataStore`, `SensorManager` y `SoilMoistureAnalog`.
Son esqueletos a completar: `TemperatureDS18B20` (1-Wire) y `WiFiMqttTransport`
(faltan librerías y credenciales, marcadas con TODO).
