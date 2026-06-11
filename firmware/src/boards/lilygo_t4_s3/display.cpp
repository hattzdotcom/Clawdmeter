#include "../../hal/display_hal.h"
#include "../../brightness.h"
#include "board.h"
#include <Arduino.h>
#include <lvgl.h>

// display_hal_init: display hardware is already started by board_init()
// via amoled.begin().  Nothing extra needed here.
void display_hal_init(void) {}

static void fill_rect(uint16_t color565) {
    // No fillScreen on LilyGo_AMOLED — push one row at a time.
    uint16_t row[LCD_WIDTH];
    for (int i = 0; i < LCD_WIDTH; i++) row[i] = color565;
    for (int y = 0; y < LCD_HEIGHT; y++) {
        amoled.pushColors(0, (uint16_t)y, LCD_WIDTH, 1, row);
    }
}

void display_hal_begin(void) {
    fill_rect(0x07E0);  // diagnostic green; change to 0x0000 once display confirmed working
    amoled.setBrightness(200);
    Serial.println("Display HAL begin OK");
}

void display_hal_set_brightness(uint8_t level) {
    amoled.setBrightness(level);
}

void display_hal_fill_screen(uint16_t color565) {
    fill_rect(color565);
}

void display_hal_draw_bitmap(int32_t x, int32_t y, int32_t w, int32_t h,
                             const uint16_t* pixels) {
    amoled.pushColors((uint16_t)x, (uint16_t)y,
                      (uint16_t)w, (uint16_t)h,
                      (uint16_t*)pixels);
}

// No rotation logic needed — fixed landscape, no IMU.
void display_hal_tick(void) {}

// RM690B0 requires even-aligned flush regions (same as CO5300).
void display_hal_round_area(int32_t* x1, int32_t* y1, int32_t* x2, int32_t* y2) {
    *x1 = *x1 & ~1;
    *y1 = *y1 & ~1;
    *x2 = *x2 | 1;
    *y2 = *y2 | 1;
}
