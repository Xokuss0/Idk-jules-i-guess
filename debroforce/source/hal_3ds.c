#ifdef __3DS__
#include <3ds.h>
#include <citro2d.h>
#include "hal/hal.h"

static C3D_RenderTarget* target_top_left;
static C3D_RenderTarget* target_bottom_scr;

void hal_init(void) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    target_top_left = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    target_bottom_scr = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
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
    C3D_FrameBegin(C3D_FRAME_SYNCHRONOUS);
}

void hal_end_frame(void) {
    C3D_FrameEnd(0);
}

void hal_select_screen(bool top_screen) {
    C2D_SceneBegin(top_screen ? target_top_left : target_bottom_scr);
}

void hal_clear_screen(unsigned char r, unsigned char g, unsigned char b) {
    u32 color = C2D_Color32(r, g, b, 255);
    C2D_TargetClear(C2D_GetTargetScreen() == GFX_TOP ? target_top_left : target_bottom_scr, color);
}

void hal_draw_rect(int x, int y, int w, int h, Color color) {
    C2D_DrawRectSolid(x, y, 0, w, h, color);
}

void hal_draw_text(int x, int y, const char* text, Color color) {
}

Texture hal_load_texture(const char* path) {
    // 3DS would use C2D_SpriteSheetLoad or similar
    // For now, this is a stub as actual sprite sheets need .t3x format
    return NULL;
}

void hal_draw_texture(Texture tex, int x, int y, int w, int h, bool flip_h) {
    // 3DS drawing using C2D_DrawImage
}

bool hal_key_pressed(Key key) {
    hidScanInput();
    u32 kDown = hidKeysDown();
    switch(key) {
        case KEY_START: return kDown & KEY_START;
        case KEY_SELECT: return kDown & KEY_SELECT;
        case KEY_UP: return kDown & KEY_UP;
        case KEY_B: return kDown & KEY_B;
        default: return false;
    }
}

bool hal_key_held(Key key) {
    u32 kHeld = hidKeysHeld();
    switch(key) {
        case KEY_LEFT: return kHeld & KEY_LEFT;
        case KEY_RIGHT: return kHeld & KEY_RIGHT;
        case KEY_A: return kHeld & KEY_A;
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
