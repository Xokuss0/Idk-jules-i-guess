#ifdef __3DS__
#include <3ds.h>
#include <citro2d.h>
#include "hal/hal.h"

static C3D_RenderTarget* target_top_left;
static C3D_RenderTarget* target_bottom_scr;
static C3D_RenderTarget* current_target;

void hal_init(void) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    target_top_left = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    target_bottom_scr = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    current_target = target_top_left;
}

void hal_exit(void) {
    C2D_Fini();
    C3D_Fini();
    gfxExit();
}

bool hal_is_running(void) {
    return aptMainLoop();
}

void hal_start_frame(void) {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
}

void hal_end_frame(void) {
    C3D_FrameEnd(0);
}

void hal_select_screen(bool top_screen) {
    current_target = top_screen ? target_top_left : target_bottom_scr;
    C2D_SceneBegin(current_target);
}

void hal_clear_screen(unsigned char r, unsigned char g, unsigned char b) {
    u32 color = C2D_Color32(r, g, b, 255);
    C2D_TargetClear(current_target, color);
}

void hal_draw_rect(int x, int y, int w, int h, Color color) {
    // Note: color is RGBA8, Citro2D expects ABGR8/RGBA8 depending on version but DrawRectSolid uses u32
    C2D_DrawRectSolid(x, y, 0, w, h, color);
}

void hal_draw_text(int x, int y, const char* text, Color color) {
}

Texture hal_load_texture(const char* path) {
    return NULL;
}

void hal_draw_texture(Texture tex, int x, int y, int w, int h, bool flip_h) {
}

bool hal_key_pressed(Key key) {
    hidScanInput();
    u32 kDown = hidKeysDown();
    switch(key) {
        case HAL_KEY_START: return kDown & KEY_START;
        case HAL_KEY_SELECT: return kDown & KEY_SELECT;
        case HAL_KEY_UP: return kDown & KEY_UP;
        case HAL_KEY_B: return kDown & KEY_B;
        default: return false;
    }
}

bool hal_key_held(Key key) {
    u32 kHeld = hidKeysHeld();
    switch(key) {
        case HAL_KEY_LEFT: return kHeld & KEY_LEFT;
        case HAL_KEY_RIGHT: return kHeld & KEY_RIGHT;
        case HAL_KEY_A: return kHeld & KEY_A;
        default: return false;
    }
}

bool hal_touch_held(TouchPos* pos) {
    touchPosition touch;
    hidTouchRead(&touch);
    if (hidKeysHeld() & KEY_TOUCH) {
        pos->x = touch.px;
        pos->y = touch.py;
        return true;
    }
    return false;
}
#endif
