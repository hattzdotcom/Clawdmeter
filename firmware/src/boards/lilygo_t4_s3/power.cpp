#include "../../hal/power_hal.h"
#include "board.h"
#include <Arduino.h>

// T4-S3 power notes:
//   - Charger IC is SY6970 (not AXP2101). Battery/charge state read via
//     LilyGo_AMOLED overrides (isCharging, isVbusIn) which delegate to SY.
//   - There is no AXP2101 PKEY IRQ. The only physical button is the boot
//     button (GPIO0), which is already polled by input_hal as INPUT_BTN_PRIMARY.
//     power_hal_pwr_* functions therefore always return false — callers that
//     need button events should use input_hal_is_held(INPUT_BTN_PRIMARY).
//   - getBatteryPercent() is not available via SY6970 without extra calibration;
//     we return -1 (unknown) so the UI hides the battery indicator.

#define BATTERY_POLL_MS  5000
#define CHARGING_POLL_MS  1000

static int      cached_pct        = -1;
static bool     cached_charging   = false;
static bool     cached_vbus       = false;
static uint32_t last_battery_ms   = 0;
static uint32_t last_charging_ms  = 0;

void power_hal_init(void) {
    cached_charging = amoled.isCharging();
    cached_vbus     = amoled.isVbusIn();
    Serial.println("Power HAL init OK (SY6970 via LilyGo)");
}

void power_hal_tick(void) {
    uint32_t now = millis();
    if (now - last_charging_ms >= CHARGING_POLL_MS) {
        last_charging_ms = now;
        cached_charging  = amoled.isCharging();
        cached_vbus      = amoled.isVbusIn();
    }
    // Battery % not available from SY6970 without calibration; leave as -1.
    (void)last_battery_ms;
}

int  power_hal_battery_pct(void)    { return cached_pct; }
bool power_hal_is_charging(void)    { return cached_charging; }
bool power_hal_is_vbus_in(void)     { return cached_vbus; }

// No AXP2101 PKEY on T4-S3 — boot button events come from input_hal.
bool power_hal_pwr_pressed(void)      { return false; }
bool power_hal_pwr_long_pressed(void) { return false; }
bool power_hal_pwr_released(void)    { return false; }
