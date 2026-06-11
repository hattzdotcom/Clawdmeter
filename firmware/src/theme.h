#pragma once
#include <lvgl.h>

// Design tokens — AMOLED dark palette (true-black bg, dark-red panels).
#define THEME_BG       lv_color_hex(0x000000)   // screen background
#define THEME_PANEL    lv_color_hex(0x2d0000)   // dark red card fill
#define THEME_TEXT     lv_color_hex(0xaaaaaa)   // primary text (medium grey)
#define THEME_DIM      lv_color_hex(0xb0b0b0)   // secondary text
#define THEME_ACCENT   lv_color_hex(0xd97757)   // terra-cotta (kept for future use)
#define THEME_GREEN    lv_color_hex(0x16a34a)   // dark green
#define THEME_AMBER    lv_color_hex(0xf59e0b)   // amber/gold
#define THEME_RED      lv_color_hex(0xef4444)   // red
#define THEME_BAR_BG   lv_color_hex(0x1a0000)   // dark red bar track
