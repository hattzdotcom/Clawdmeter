#include "board.h"
#include <Arduino.h>

// Single LilyGo_AMOLED instance shared across all HAL files for this board.
// begin() initialises display, touch, and PMU in one call.
LilyGo_AMOLED amoled;

extern "C" void board_init(void) {
    // PMIC enable must be asserted before the display and I2C peripherals
    // are powered.  The LilyGo library normally handles this inside begin(),
    // but driving it early ensures a clean power-up even if begin() is
    // called before Serial is ready.
    pinMode(PMIC_EN_PIN, OUTPUT);
    digitalWrite(PMIC_EN_PIN, HIGH);

    // disable_sd=true: no SD slot populated, skip SPI bus init for card
    if (!amoled.beginAMOLED_241(true)) {
        Serial.println("LilyGo AMOLED init FAILED");
    }

    // RM690B0 is natively landscape: rotation 0 = 600×450.  No setRotation call
    // needed — beginAMOLED_241 already calls setRotation(0).

    Serial.printf("LilyGo T4-S3 init OK (%d×%d)\n",
                  amoled.width(), amoled.height());
}
