#pragma once
#include <lvgl.h>

// Design tokens — AMOLED dark palette (near-black bg, muted dark-red tones).
#define THEME_BG        lv_color_hex(0x0A0A0A)  // near-black screen background
#define THEME_PANEL     lv_color_hex(0x1A1212)  // very dark warm card fill
#define THEME_TEXT      lv_color_hex(0xBF4F4F)  // muted brick red — usage screen text
#define THEME_DIM       lv_color_hex(0x7A3535)  // darker muted red — secondary text
#define THEME_ACCENT    lv_color_hex(0xBF4F4F)  // same as text (kept for future use)
#define THEME_GREEN     lv_color_hex(0x3D6B3D)  // muted forest green  (bar 0–50 %)
#define THEME_AMBER     lv_color_hex(0x7A6630)  // muted golden amber  (bar 51–75 %)
#define THEME_RED       lv_color_hex(0x7A2828)  // muted dark red      (bar > 75 %)
#define THEME_BAR_BG    lv_color_hex(0x1E1414)  // dark warm bar track
#define THEME_CLOCK     lv_color_hex(0x7A1A1A)  // dark red — screensaver time
#define THEME_CLOCK_DIM lv_color_hex(0x4D1010)  // very dark red — screensaver date
