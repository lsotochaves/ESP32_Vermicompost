# Comparativa de sensores de O₂ para el monitor de vermicompost

Documento de decisión para la selección (o descarte) de un sensor de oxígeno en el nodo de
monitoreo de vermicompost. Se comparan los dos candidatos evaluados:

- **DFRobot Gravity SEN0322** — celda electroquímica (galvánica), I²C.
- **SST Sensing LuminOx LOX-02** — óptico (luminescence quenching), UART.

> **Criterio de fuentes:** los datos marcados **Oficial** provienen directamente del
> datasheet del fabricante (confianza >98 %). Los marcados **Derivado / Principio /
> Comunidad / Fabricante** corresponden a análisis de ingeniería o fuentes secundarias,
> indicados caso por caso. **n/d** = el fabricante no lo publica.
>
> Última actualización: 2026-07-22.

---

## 1. Resumen ejecutivo

- Como señal de aireación y estado aeróbico, el O₂ constituye un indicador directo: detecta
  anaerobiosis y mide el oxígeno disponible real.
- **El LuminOx (óptico) es técnicamente superior** para un sistema desatendido: no se
  consume por exposición, no requiere recalibración manual, presenta **cross-sensitivity nula
  a H₂S y CO₂ según su tabla oficial**, y su degradación solo avanza durante la medición
  (compatible con deep sleep). Su costo es el principal inconveniente. El rango térmico de
  operación presenta una discrepancia entre documentos oficiales de SST (+10 a +45 °C en el
  datasheet DS0144; −30 a +60 °C en el user's guide UG-001), a verificar con el fabricante.
- **El SEN0322 (galvánico) presenta menor costo**, pero implica un consumible (~2 años desde
  el desellado, con o sin medición), deriva (<2 %/mes), **recalibración manual periódica al
  aire libre** y un **tiempo de estabilización oficial de ≈3–10 min por medición**, factores
  incompatibles con la operación desatendida y con el presupuesto energético de deep sleep
  del proyecto.

---

## 2. Tabla comparativa — especificaciones oficiales (datasheet)

| Variable | SEN0322 (galvánico) | LuminOx LOX-02 (óptico) | Fuente |
|---|---|---|---|
| Principio | Electroquímico (galvánico) | Óptico (luminescence quenching), *NOT electrochemical* | Oficial ambos |
| Rango | 0–25 % Vol | 0–25 % O₂ (0–300 mbar ppO₂) | Oficial ambos |
| Máximo de medición | **30 % Vol** | — (no especificado) | Oficial SEN0322 |
| Resolución | 0.15 % Vol | 0.01 % O₂ | Oficial ambos |
| Exactitud | No indica "accuracy"; repetibilidad <2 % | ppO₂ < 2 % FS | Oficial ambos |
| Cross-sensitivity (gases probados) | n/d (DFRobot no publica tabla) | **None** en H₂S 100 ppm, CO₂ 100 %, SO₂, HCl, CO, CH₄, NO, etileno, acetaldehído | Oficial LuminOx |
| Sensibilidad | (0.10 ± 0.05) mA (en aire) | — | Oficial SEN0322 |
| Estabilidad / deriva | **<2 % por mes** | No especifica deriva; calibrado de fábrica, "maintenance free" | Oficial ambos |
| Response time | **≤15 s** | **T90 < 30 s (típico)** | Oficial ambos |
| Tiempo de estabilización (warm-up) | **≈3–10 min** (datasheet/código de ejemplo) | Corto (lectura cada 1 s) | Oficial SEN0322 |
| Temp. operación | **−20 a 50 °C** | **+10 a +45 °C** (datasheet DS0144); −30 a +60 °C (user's guide UG-001) ⚠️ | Oficial ambos |
| Humedad operación | 0–99 % RH sin condensación | 0–99 % RH sin condensación | Oficial ambos |
| Presión de operación | Presión atmosférica estándar ±10 % | 500–1200 mbar | Oficial ambos |
| Voltaje alimentación | 3.3–5.5 V DC | **4.5–5.5 V DC** (requiere 5 V) | Oficial ambos |
| Corriente | **n/d** (DFRobot no la publica) | <7.5 mA (stream 1/s), <20 mA pico, **<6 mA en Off Mode** | LuminOx oficial |
| Salida digital | I²C (nivel según VCC) | USART TTL **3.3 V** | Oficial ambos |
| Dirección / bus | I²C 0x70–0x73 (def. 0x73, ADDRESS_3); V1.0 solo 0x72/0x73 | UART (no direccionable) | Oficial SEN0322 |
| Vida útil | **>2 años (en aire)** | **>2 años** | Oficial ambos |
| Filtro / protección | **Membrana impermeable y transpirable integrada** ("no abrir ni dañar"); sin filtro químico | **Filtro PTFE integrado** ("do NOT remove") | Oficial ambos |
| Materiales | Celda con electrolito (riesgo de fuga); contiene plomo | Sin materiales peligrosos, RoHS/REACH | LuminOx oficial; SEN0322 datasheet/principio |
| Dimensiones | 37 × 27 × 24.5 mm | Ø20 × 12.5 mm | Oficial ambos |
| Peso | 0.037 kg | — | Oficial SEN0322 |

---

## 3. Tabla comparativa — variables relevantes para vermicompost (evaluación)

> Estas filas **no** constituyen datos de datasheet: corresponden a análisis de ingeniería o
> a comportamiento de principio. Se indica la base de cada una.

| Variable | SEN0322 (galvánico) | LuminOx LOX-02 (óptico) | Base |
|---|---|---|---|
| Consumo por exposición | Sí — envejece por exposición al aire, con o sin medición | No — solo envejece durante la medición | Principio galvánico vs óptico ¹ |
| ¿Deep sleep prolonga su vida? | No | Sí (en reposo no envejece) | Derivado del principio ¹ |
| Recalibración manual periódica | **Sí** (aire libre, 20.9 % O₂) | No (calibrado de fábrica) | Datasheet + práctica ² |
| Cross-sensitivity / gases corrosivos | Afectable; el datasheet advierte que los gases corrosivos dañan el sensor | **Inmune — tabla oficial: H₂S 100 ppm y CO₂ 100 % = None** | Datasheet ambos ³ |
| Riesgo por condensación | Medio (membrana impermeable/transpirable integrada; opera "sin condensación") | Menor (filtro PTFE; aún "sin condensación") | Datasheet ⁴ |
| Warm-up / estabilización | **≈3–10 min (OFICIAL, datasheet/código)** | Corto (T90 <30 s, lectura cada 1 s) | Datasheet ambos ⁵ |
| Compatibilidad con rango térmico del lecho | Amplio (−20–50 °C) | Discrepancia entre docs SST: +10–45 °C (datasheet) vs. −30–60 °C (user's guide) | Datasheet + UG ⁷ |
| Integración con la arquitectura del repositorio | I²C (como `SoilMoistureAnalog`) | UART RS232 3.3 V, 9600 8N1; modos Stream/Poll/Off; requiere 5 V | `ISensor` del repositorio |
| Precio | Menor | Mayor (varias veces) | **No oficial — verificar con distribuidor** ⁶ |
| Idoneidad global (operación desatendida) | Media (consumible + recalibración + deriva) | Alta (sin mantenimiento, inmune a gases) | Síntesis |

**Notas de fuente**

1. Comportamiento intrínseco de las celdas galvánicas frente a las ópticas (principio físico
   general). DFRobot lo sugiere con *"Lifetime >2 años **in the air**"*. No es cifra de
   datasheet.
2. **Oficial:** el datasheet del SEN0322 especifica la calibración al aire libre (O₂
   atmosférico = 20.9 %) mediante pulsación larga del botón de calibración durante 2 s, una
   vez estabilizada la lectura. El LuminOx se declara *"Factory calibrated / Maintenance
   free"* (oficial), con la salvedad de su datasheet: puede requerir recalibración tras
   fluctuaciones térmicas extremas.
3. **Oficial (LuminOx User's Guide UG-001):** el LuminOx incluye una tabla de
   cross-sensitivity en la que H₂S (100 ppm), CO₂ (100 %), SO₂, HCl, CO, CH₄, NO, etileno y
   acetaldehído dan resultado *None*; la tecnología de fluorescencia se describe como
   "extremely selective to O₂". **El datasheet del SEN0322 sí advierte oficialmente** que las
   celdas electroquímicas no deben usarse en ambientes con gases corrosivos ni almacenarse en
   gas ácido de alta concentración; DFRobot no publica una tabla numérica de cross-sensitivity
   para el SEN0322.
4. Ambos datasheets indican "sin condensación". El SEN0322 incorpora una membrana
   impermeable y transpirable (sin filtro químico); el LuminOx incorpora filtro PTFE
   integrado (oficial ambos).
5. **Oficial:** el código de ejemplo del datasheet del SEN0322 indica *"it takes time to
   stable oxygen concentration, about 10 minutes"*, y la sección *Expected Results* menciona
   ~3 min de calentamiento; se adopta ≈3–10 min. Aun así, conviene medir el warm-up real
   desde deep sleep en el montaje definitivo.
6. Precios no oficiales y variables según distribuidor y país. Verificar en Mouser, DFRobot o
   GasLab antes de la compra.
7. **Discrepancia entre documentos oficiales de SST:** el datasheet DS0144 (Rev 4) indica
   temperatura de operación +10 a +45 °C, mientras que el user's guide UG-001 (Rev 1, 2016)
   indica −30 a +60 °C. Se recomienda diseñar con el rango más conservador (+10 a +45 °C) y
   confirmar con el fabricante. Para vermicompost (lecho típico 15–30 °C) ambos rangos suelen
   ser compatibles.

---

## 4. Uso documentado en proyectos reales de vermicompost


### 4.2. Sobre el LuminOx
- **Observación relevante:** el propio fabricante (SST Sensing), en su artículo sobre
  compostaje, **no recomienda el LuminOx**, sino su analizador industrial **OXY-FLEX
  (zirconio)** para instalaciones de compostaje. El fabricante no posiciona el LuminOx para
  este uso.

### 4.3. Contexto documentado (sin uso directo de estos sensores)
- El **monitoreo de O₂ en compostaje está validado científicamente** (respirometría /
  *Oxygen Uptake Rate*, OUR), pero mediante sensores **industriales o a medida**, no con
  estos módulos:
  - Microsistema respirométrico para compost: emplea **sensores galvánicos a medida**
    (cátodo de platino, ánodo de plomo, electrolito KOH) y, como referencia, el sensor
    comercial **AlphaSense AP-0001 O2-A2** — no el SEN0322 ni el LuminOx.
  - Optimización de la aireación en compostaje de lodos: equipo de monitoreo de O₂ dedicado.
  - SST recomienda el **OXY-FLEX** (zirconio) para compostaje.
- Los **proyectos IoT reales de vermicompost** (ESP32 / Raspberry Pi) monitorean
  **temperatura, humedad, pH y gases mediante sensores MQ**; **no** emplean un sensor de O₂
  dedicado, ni el SEN0322 ni el LuminOx. Ejemplos:
  - Subendran et al. (2024), "IoT-Based Vermicompost Monitoring on Basis of Moisture, Gas
    and Temperature using ESP32" — mide humedad, gas (MQ) y temperatura.
  - Sistema de vermicompostaje solar de lazo cerrado con IoT/Raspberry Pi — temperatura y
    humedad.

### 4.4. Implicación para el proyecto (TCU)
- No existe un **precedente establecido** que replicar con estos sensores; el proyecto se
  situaría en terreno relativamente original al emplear un sensor de O₂ dedicado en
  vermicompost.
- El **respaldo científico del monitoreo de O₂ en compostaje existe** (OUR / respirometría),
  por lo que el planteamiento es sólido; lo que falta es la implementación de bajo costo
  concreta con estos módulos.
- **Recomendación:** aprovechar la ausencia de precedente documentando la validación propia
  en `hardware_tests/` y `Pruebas.md` (tiempo de estabilización real, deriva en humedad alta,
  comportamiento ante condensación), generando el dato que actualmente no existe.

---

## 5. Mitigación de la interferencia por otros gases (cross-sensitivity)

> El SEN0322 (galvánico) es sensible a los gases del vermicompost (CO₂, NH₃, H₂S). El LuminOx
> (óptico) es inmune, lo cual está confirmado por su tabla oficial de cross-sensitivity
> (H₂S 100 ppm y CO₂ 100 % = *None*; UG-001). Esta sección documenta las técnicas de
> mitigación aplicables al sensor galvánico, empleadas en la literatura y por los fabricantes,
> con sus fuentes. **Todas las técnicas listadas cuentan con respaldo en literatura o en notas
> de fabricante.**

### 5.1. Efecto documentado
- En los sensores de O₂ **galvánicos**, gases como **CO₂, He, Ar e H₂ incrementan la
  difusión de O₂ y elevan la corriente de salida**, produciendo una interferencia
  **positiva** (lectura de O₂ superior a la real). Es un efecto de difusión documentado por
  AlphaSense (fabricante de sensores de O₂ galvánicos, incluido el sensor de referencia
  empleado en la respirometría de compost del §4.3).

### 5.2. Técnicas de mitigación documentadas

| Técnica | Interferencia que mitiga | Fuente / respaldo | Limitación documentada |
|---|---|---|---|
| **Filtros químicos / capas de filtro** (carbón activado, zeolitas, tamices moleculares, sustratos fluorados) | Gases ácidos y reactivos (H₂S, SO₂, VOCs) | Artículo revisado por pares (supresión de cross-sensitivity NO/SO₂ con capas de filtro); white paper de Testo; Industrial Scientific | Vida útil finita; pierden efectividad antes que el sensor; suelen **reducir la sensibilidad** |
| **Filtro de tela/fibra de carbón sobre el electrodo** | VOCs; gases de combustión | AlphaSense O₂-C2 (emplea carbon cloth filter en gases agresivos) | Consumible |
| **Scrubber de CO₂ con cal sodada (soda lime) o Ascarite** | CO₂ | Método estándar de respirometría (LI-COR, Sable Systems, respirometría de compost) | Se satura (capacidad ~20–30 % W/W); consumible |
| **Compensación por software** (medición del gas interferente y corrección) | El gas interferente medido de forma independiente | Práctica de fabricantes (Testo, Industrial Scientific) | Requiere un segundo sensor y un modelo calibrado |

> **Nota aplicada al dominio:** en respirometría de compost, el aislamiento de la señal
> O₂/CO₂ es práctica resuelta — se emplea cal sodada o Ascarite para que los cambios de gas
> reflejen el consumo de O₂ y no la producción de CO₂. Es la técnica más directamente
> aplicable al proyecto.

### 5.3. Consenso de las fuentes
- No existe un filtro perfecto: *"there is not a perfect technology that can filter out all
  of the interfering gases"* (Aeroqual / literatura de sensores electroquímicos).
- Toda mitigación aplicada sobre el sensor galvánico introduce **un consumible o complejidad
  adicional**, lo que contradice el objetivo de operación desatendida. La única solución
  **de raíz** es el principio óptico (LuminOx), inmune sin necesidad de filtros.

### 5.4. Estrategia para ampliar la investigación
- **Bases de datos:** Google Scholar, ScienceDirect, IEEE Xplore, MDPI *Sensors*.
- **Términos (en inglés):** `electrochemical oxygen sensor cross-sensitivity`,
  `interfering gas chemical filter/scrubber`, `activated carbon filter gas sensor selectivity`,
  `soda lime CO2 scrubber respirometry`, `galvanic O2 sensor CO2 interference`.
- **Notas de aplicación de fabricantes:** `AlphaSense application note oxygen`,
  `Membrapor O2 cross-sensitivity`, `SGX Sensortech O2`, `City Technology oxygen sensor`.
- **Por número de parte:** consultar la *cross-sensitivity table* del sensor concreto
  (por ejemplo, `AlphaSense O2-A2 cross sensitivity`). DFRobot **no** publica esta tabla para
  el SEN0322; se recomienda emplear como proxy los datos de sensores galvánicos equivalentes
  (AlphaSense / Membrapor).
- **En el dominio de aplicación:** `compost respirometry CO2 scrubber method`.

---

## 6. Recomendación y decisión

### 6.1. Síntesis del análisis
Con los datos oficiales de ambos datasheets incorporados, el balance técnico se resume así:

| Criterio (operación desatendida) | Ventaja |
|---|---|
| Ausencia de consumible / mantenimiento | **LuminOx** (no se consume; sin recalibración manual) |
| Inmunidad a gases del vermicompost (H₂S, CO₂) | **LuminOx** (tabla oficial: *None*) |
| Compatibilidad con deep sleep | **LuminOx** (listo <30 s; degradación solo al medir) vs. SEN0322 (warm-up oficial 3–10 min; envejece aunque no mida) |
| Resistencia a gases corrosivos | **LuminOx** (el datasheet del SEN0322 desaconseja ambientes corrosivos) |
| Integración con la arquitectura del repositorio | **SEN0322** (I²C, como los sensores actuales; 3.3 V) |
| Costo | **SEN0322** (menor) |

- En **prestaciones técnicas para este entorno**, el LuminOx es superior en la mayoría de
  criterios; el SEN0322 solo aventaja en costo y en facilidad de integración (I²C nativo a
  3.3 V frente al UART a 5 V del LuminOx).
- Ningún sensor cuenta con precedente documentado en vermicompost (§4), por lo que la
  validación propia es necesaria en cualquier caso.

### 6.2. Recomendación
- **Si el presupuesto lo permite: LuminOx LOX-02.** Es la opción alineada con un sistema
  desatendido y de larga duración: sin consumible, sin recalibración manual, inmune a los
  gases del compost y compatible con el ciclo de deep sleep. Requiere resolver la
  alimentación a 5 V y la lectura por UART.
- **Si el costo es la restricción dominante: SEN0322**, asumiendo explícitamente su plan de
  mantenimiento (recalibración periódica al aire libre, reemplazo de la celda ~cada 2 años),
  el impacto energético del warm-up de 3–10 min por medición, y una mitigación de
  cross-sensitivity documentada (§5) si se confirma interferencia relevante.
- **Antes de comprometer cualquier compra**, resolver los puntos pendientes de §7 (en
  especial la corriente del SEN0322 y el tiempo total real de medición), ya que pueden
  descartar un candidato por presupuesto energético con independencia de sus prestaciones.

### 6.3. Decisión
> *Pendiente de confirmación por parte del responsable del proyecto.* Registrar aquí la
> opción seleccionada, la fecha y la justificación una vez tomada la decisión.

---

## 7. Datos pendientes de verificar (antes de decidir la compra)

- [ ] **Corriente de operación del SEN0322** (DFRobot no la publica) — medir, para el
      presupuesto de energía.
- [ ] **Warm-up real del SEN0322 desde deep sleep** — el datasheet indica ≈3–10 min;
      verificar el valor real en el montaje definitivo en `hardware_tests/`.
- [ ] **Tiempo total real de medición** (warm-up + difusión del gas en la cámara + T90) en el
      montaje físico definitivo, para ambos candidatos.
- [ ] **Precio actual** 
- [ ] **Rango térmico del lecho** en el sitio real frente al límite +10 °C del LuminOx.

---

## 8. Fuentes

**Oficiales (datasheet / fabricante):**
- SEN0322 — Datasheet oficial DFRobot (SKU:SEN0322; archivo local
  `Datasheet/A700000013081480.pdf`, export de la wiki). Producto:
  <https://www.dfrobot.com/product-2052.html>
- SEN0322 — DFRobot Wiki (tabla de especificaciones):
  <https://wiki.dfrobot.com/Gravity_I2C_Oxygen_Sensor_SKU_SEN0322>
- LuminOx LOX-02-S — SST Sensing Datasheet DS-0144 Rev 3 (© 2022):
  <https://sstsensing.com/wp-content/uploads/2025/06/DS0144_Rev4-LOX-02S.pdf>
- LuminOx — User's Guide oficial SST Sensing UG-001 Rev 1 (© 2016; archivo local
  `Datasheet/Manual-Luminox-LOX-02-CO2-Sensor.pdf`). Incluye la tabla de cross-sensitivity,
  el set de comandos RS232 y los modos Stream/Poll/Off.
- SST Sensing — "Optimising Composting with Precision Oxygen Monitoring" (recomienda
  OXY-FLEX, no LuminOx): <https://sstsensing.com/optimising-composting-with-precision-oxygen-monitoring/>

**Secundarias / contexto:**
- Subendran et al. (2024), "IoT-Based Vermicompost Monitoring on Basis of Moisture, Gas and
  Temperature using ESP32" (SSRN): <https://papers.ssrn.com/sol3/papers.cfm?abstract_id=4986848>
- "Portable and Low-Cost Respirometric Microsystem for the Static and Dynamic Respirometry
  Monitoring of Compost" (PMC6806091):
  <https://pmc.ncbi.nlm.nih.gov/articles/PMC6806091/>
- "Oxygen Monitoring Equipment for Sewage-Sludge Composting and Its Application to Aeration
  Optimization" (PMC6264128): <https://pmc.ncbi.nlm.nih.gov/articles/PMC6264128/>
- Librería DFRobot_OxygenSensor (GitHub): <https://github.com/DFRobot/DFRobot_OxygenSensor>

**Mitigación de cross-sensitivity (§5):**
- Artículo revisado por pares — "Suppression of NO and SO₂ cross-sensitivity in
  electrochemical CO₂ sensors with filter layers":
  <https://www.researchgate.net/publication/257353714_Suppression_of_NO_and_SO2_cross-sensitivity_in_electrochemical_CO2_sensors_with_filter_layers>
- Artículo — "Activated- and temperature-controlled carbon filter to avoid interferences of
  ozone and nitrogen dioxide on semiconducting gas sensors" (ScienceDirect):
  <https://www.sciencedirect.com/science/article/abs/pii/S0925400510006787>
- Testo — White paper cross-sensitivity compensation:
  <https://www.testo.com/en-US/downloads/emission-whitepaper-cross-sensitivities-registration>
- Industrial Scientific — Electrochemical Gas Sensor Cross Interference Table:
  <https://www.indsci.com/en/blog/electrochemical-sensor-cross-interference-table>
- AlphaSense — O₂-C2 (galvánico con carbon cloth filter):
  <https://www.alphasense.com/products/view-by-target-gas/o2-c2-oxygen-sensor>
- AlphaSense — Application Note AAN 104:
  <https://www.alphasense.com/-/media/project/oneweb/oneweb/alphasense/products/application-notes/aan_104_app-note_v0.pdf>
- LI-COR — Tech Tip: Using CO₂ and H₂O Scrubbers with Gas Analyzers:
  <https://www.licor.com/support/TechTips/doc-08311-scrubbers-note.html>
- Sable Systems — Gas Sensors for Respirometry:
  <https://www.sablesys.com/gas-sensors-for-respirometry/>
- Atlas Scientific — Soda Lime (CO₂): <https://atlas-scientific.com/co2/soda-lime/>
- "Development of an automated, multi-vessel respirometric system to evaluate decomposition
  of composting feedstocks" (ScienceDirect):
  <https://www.sciencedirect.com/science/article/pii/S1537511022002446>
