#include "splash.h"
#include "splash_animations.h"
#include "theme.h"
#include "usage_rate.h"
#include "hal/board_caps.h"
#include <Arduino.h>
#include <time.h>
#include <string.h>
#include <esp_heap_caps.h>

LV_FONT_DECLARE(font_styrene_48);
LV_FONT_DECLARE(font_styrene_28);

// ---- Clock screen ----

static lv_obj_t *splash_container = NULL;
static lv_obj_t *lbl_time = NULL;
static lv_obj_t *lbl_date = NULL;
static bool      active = false;
static uint32_t  last_clock_ms = 0;

static const char* const MONTH_NAMES[] = {
    "JAN","FEB","MAR","APR","MAY","JUN",
    "JUL","AUG","SEP","OCT","NOV","DEC"
};

static void update_clock(void) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char time_buf[12];
    snprintf(time_buf, sizeof(time_buf), "%02d:%02d:%02d",
             t->tm_hour, t->tm_min, t->tm_sec);

    char date_buf[14];
    snprintf(date_buf, sizeof(date_buf), "%s %02d %04d",
             MONTH_NAMES[t->tm_mon], t->tm_mday, 1900 + t->tm_year);

    if (lbl_time) lv_label_set_text(lbl_time, time_buf);
    if (lbl_date) lv_label_set_text(lbl_date, date_buf);
}

// ---- Mini animated creature (used by the idle sub-screen in ui.cpp) ----

#define GRID 20

static lv_obj_t  *mini_canvas = NULL;
static uint16_t  *mini_buf    = NULL;
static int        mini_cell   = 0;
static int        mini_w      = 0;
static const splash_anim_def_t *mini_anim  = NULL;
static uint16_t   mini_frame  = 0;
static uint32_t   mini_started = 0;

static void mini_render(void) {
    if (!mini_buf || !mini_anim) return;
    const uint8_t  *cells = mini_anim->frames[mini_frame];
    const uint16_t *pal   = mini_anim->palette;
    for (int gy = 0; gy < GRID; gy++) {
        for (int gx = 0; gx < GRID; gx++) {
            uint8_t  code  = cells[gy * GRID + gx];
            uint16_t color = (pal && code < SPLASH_PALETTE_SIZE) ? pal[code] : 0x0000;
            for (int dy = 0; dy < mini_cell; dy++) {
                uint16_t *dst = &mini_buf[(gy * mini_cell + dy) * mini_w + gx * mini_cell];
                for (int dx = 0; dx < mini_cell; dx++) dst[dx] = color;
            }
        }
    }
    if (mini_canvas) lv_obj_invalidate(mini_canvas);
}

lv_obj_t* splash_mini_create(lv_obj_t *parent, const char *anim_name, int px) {
    mini_anim = NULL;
    for (int i = 0; i < SPLASH_ANIM_COUNT; i++) {
        if (strcmp(splash_anims[i].name, anim_name) == 0) {
            mini_anim = &splash_anims[i];
            break;
        }
    }
    if (!mini_anim) return NULL;

    mini_cell = px / GRID;
    if (mini_cell < 1) mini_cell = 1;
    mini_w = GRID * mini_cell;

#ifdef BOARD_HAS_PSRAM
    const uint32_t caps = MALLOC_CAP_SPIRAM;
#else
    const uint32_t caps = MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT;
#endif
    mini_buf = (uint16_t*)heap_caps_malloc(mini_w * mini_w * 2, caps);
    if (!mini_buf) return NULL;

    mini_canvas = lv_canvas_create(parent);
    lv_canvas_set_buffer(mini_canvas, mini_buf, mini_w, mini_w, LV_COLOR_FORMAT_RGB565);
    mini_frame   = 0;
    mini_started = millis();
    mini_render();
    return mini_canvas;
}

void splash_mini_tick(void) {
    if (!mini_buf || !mini_anim || mini_anim->frame_count == 0) return;
    if (millis() - mini_started < mini_anim->holds[mini_frame]) return;
    mini_started = millis();
    mini_frame   = (mini_frame + 1) % mini_anim->frame_count;
    mini_render();
}

// ---- Public API ----

void splash_init(lv_obj_t *parent) {
    const BoardCaps& c = board_caps();

    splash_container = lv_obj_create(parent);
    lv_obj_set_size(splash_container, c.width, c.height);
    lv_obj_set_pos(splash_container, 0, 0);
    lv_obj_set_style_bg_color(splash_container, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(splash_container, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(splash_container, 0, 0);
    lv_obj_set_style_pad_all(splash_container, 0, 0);
    lv_obj_clear_flag(splash_container, LV_OBJ_FLAG_SCROLLABLE);

    lbl_time = lv_label_create(splash_container);
    lv_label_set_text(lbl_time, "--:--:--");
    lv_obj_set_style_text_font(lbl_time, &font_styrene_48, 0);
    lv_obj_set_style_text_color(lbl_time, lv_color_hex(0xef4444), 0);
    lv_obj_align(lbl_time, LV_ALIGN_CENTER, 0, -35);

    lbl_date = lv_label_create(splash_container);
    lv_label_set_text(lbl_date, "--- -- ----");
    lv_obj_set_style_text_font(lbl_date, &font_styrene_28, 0);
    lv_obj_set_style_text_color(lbl_date, lv_color_hex(0xef4444), 0);
    lv_obj_align(lbl_date, LV_ALIGN_CENTER, 0, 40);

    lv_obj_add_flag(splash_container, LV_OBJ_FLAG_HIDDEN);
}

void splash_tick(void) {
    if (!active) return;
    uint32_t now = millis();
    if (now - last_clock_ms < 1000) return;
    last_clock_ms = now;
    update_clock();
}

void splash_next(void) {}

void splash_pick_for_current_rate(void) {}

bool splash_is_active(void) { return active; }

void splash_show(void) {
    update_clock();
    last_clock_ms = millis();
    if (splash_container) lv_obj_clear_flag(splash_container, LV_OBJ_FLAG_HIDDEN);
    active = true;
}

void splash_hide(void) {
    if (splash_container) lv_obj_add_flag(splash_container, LV_OBJ_FLAG_HIDDEN);
    active = false;
}

lv_obj_t* splash_get_root(void) {
    return splash_container;
}
