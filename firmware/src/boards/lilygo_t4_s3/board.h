#pragma once

// LilyGo T4-S3 — 2.41" RM690B0 AMOLED, 450×600 native (portrait).
// We run in landscape via setRotation(1): logical 600×450.
// All peripherals accessed through the global `amoled` LilyGo_AMOLED instance.

#include <LilyGo_AMOLED.h>

// The single board-wide peripheral instance. Defined in board_init.cpp;
// display.cpp, touch.cpp, and power.cpp reference it via this extern.
extern LilyGo_AMOLED amoled;

#define BOARD_NAME  "LilyGo T4-S3"

// Logical dimensions after landscape rotation.
#define LCD_WIDTH   600
#define LCD_HEIGHT  450

// ---- QSPI display pins (RM690B0) ----
#define LCD_D0      14
#define LCD_D1      10
#define LCD_D2      16
#define LCD_D3      12
#define LCD_SCLK    15
#define LCD_CS      11
#define LCD_RESET   13
#define LCD_TE      18

// ---- Shared I2C bus (touch + PMU) ----
#define IIC_SDA     6
#define IIC_SCL     7

// ---- Touch (CHSC5816) ----
#define TP_INT      8
#define TP_RST      17

// ---- PMU (AXP2101) IRQ ----
#define PMU_IRQ     5

// ---- PMIC enable pin (must be driven HIGH before display init) ----
#define PMIC_EN_PIN 9

// ---- Physical button ----
#define BTN_BACK_GPIO  0   // BOOT — INPUT_BTN_PRIMARY (PTT / Space)

// ---- Capability flags (compile-time dead-stripping guards) ----
#define BOARD_HAS_SECONDARY_BUTTON  0
#define BOARD_HAS_ROTATION          0   // fixed landscape, no IMU
#define BOARD_HAS_IMU               0
#define BOARD_HAS_BATTERY           1   // AXP2101 present; battery optional
