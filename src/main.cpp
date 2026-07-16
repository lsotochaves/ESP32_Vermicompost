#include <Arduino.h>
#include "core/Reading.h"
#include "Scheduler.h"
#include "PowerManager.h"
#include "SensorManager.h"
#include "RtcDataStore.h"
#include "SoilMoistureAnalog.h"
// #include "TemperatureDS18B20.h"
#include "WiFiMqttTransport.h"

using namespace vermi;

// ----------------------- Configuration (edit here) -----------------------
static const ScheduleConfig SCHED = {
    /* sampleIntervalSec */ 900,    // measure every 15 min
    /* sendIntervalSec   */ 21600,  // upload every 6 h  (24 samples per upload)
};

// With deep sleep there is no persistent loop: each wake re-runs setup() from
// scratch. So the whole cycle lives here, ending in deepSleepFor() which never
// returns. Persistent state (buffer, timing) lives in RTC memory inside the
// Scheduler and RtcDataStore.
void setup() {
    Serial.begin(115200);
    delay(50);

    // 1) Decide what to do this wake.
    Scheduler  scheduler(SCHED);
    WakeAction action = scheduler.onWake();
    uint32_t   now    = scheduler.nowSec();

    // 2) Build the sensor set. Adding a future sensor = one more add() line.
    SensorManager sensors;
    static SoilMoistureAnalog soil(METRIC_SOIL_MOIST_PCT, /*ADC1 pin*/ 34,
                                   /*dryRaw*/ 3200, /*wetRaw*/ 1200);
    sensors.add(&soil);
    // static TemperatureDS18B20 temp(METRIC_TEMPERATURE_C, /*1-Wire pin*/ 4);
    // sensors.add(&temp);
    sensors.beginAll();

    // 3) Sample into a transient buffer, then persist to RTC memory.
    Reading scratch[16];
    size_t got = sensors.sampleAll(scratch, 16, now);

    RtcDataStore store;
    for (size_t i = 0; i < got; ++i) store.append(scratch[i]);

    // 4) Only on a send cycle do we power the radio.
    if (action == WakeAction::SampleAndSend && store.count() > 0) {
        WiFiMqttTransport transport;
        if (transport.connect() && transport.send(store.data(), store.count())) {
            store.clear();             // flushed successfully
        }
        transport.disconnect();        // radio off no matter what
    }

    // 5) Sleep until the next event. Does not return.
    uint32_t sleepSec = scheduler.secondsUntilNextWake();
    Serial.printf("[vermi] now=%us action=%d buffered=%u -> sleep %us\n",
                  now, (int)action, (unsigned)store.count(), sleepSec);
    Serial.flush();
    PowerManager::deepSleepFor(sleepSec);
}

void loop() { /* never reached: deep sleep reboots into setup() */ }
