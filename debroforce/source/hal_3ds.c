#ifdef __3DS__
#include <3ds.h>
#include <citro2d.h>
#include "hal/hal.h"

static C3D_RenderTarget* top;
static C3D_RenderTarget* bottom;

void hal_init(void) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
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
    C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
    C2D_SceneBegin(top);
}

void hal_end_frame(void) {
    C3D_FrameEnd(0);
}

void hal_draw_rect(int x, int y, int w, int h, Color color) {
    C2D_DrawRectSolid(x, y, 0, w, h, color);
}

void hal_draw_text(int x, int y, const char* text, Color color) {
    // Basic text drawing would need C2D_TextBuf etc.
    // This is just a stub for now.
}

bool hal_key_pressed(Key key) {
    hidScanInput();
    u32 kDown = hidKeysDown();
    switch(key) {
        case KEY_START: return kDown & KEY_START;
        default: return false;
    }
}

bool hal_key_held(Key key) {
    u32 kHeld = hidKeysHeld();
    switch(key) {
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
