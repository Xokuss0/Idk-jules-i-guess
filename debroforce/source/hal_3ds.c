#ifdef __3DS__
#include <3ds.h>
#include <citro2d.h>
#include "hal/hal.h"

static C3D_RenderTarget* target_top_left;
static C3D_RenderTarget* target_bottom_scr;
static C3D_RenderTarget* current_target;
static int cam_x = 0;
static int cam_y = 0;

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
    hidScanInput();
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

void hal_set_camera(int x, int y) {
    cam_x = x;
    cam_y = y;
}

void hal_draw_rect(int x, int y, int w, int h, Color color) {
    int rx = (current_target == target_top_left) ? (x - cam_x) : x;
    int ry = (current_target == target_top_left) ? (y - cam_y) : y;
    C2D_DrawRectSolid(rx, ry, 0, w, h, color);
}

void hal_draw_text(int x, int y, const char* text, Color color) {
}

Texture hal_load_texture(const char* path) {
    // On 3DS this would load a .t3x or from a sprite sheet.
    // Stubbing for now to allow compilation.
    return NULL;
}

void hal_draw_texture(Texture tex, int x, int y, int w, int h, bool flip_h) {
    if (!tex) return;
    int rx = (current_target == target_top_left) ? (x - cam_x) : x;
    int ry = (current_target == target_top_left) ? (y - cam_y) : y;
    // C2D_DrawImage(..., rx, ry, ...)
}

bool hal_key_pressed(Key key) {
    u32 kDown = hidKeysDown();
    circlePosition cpad;
    hidCircleRead(&cpad);
    switch(key) {
        case HAL_KEY_START: return kDown & KEY_START;
        case HAL_KEY_SELECT: return kDown & KEY_SELECT;
        case HAL_KEY_UP: return (kDown & (KEY_UP | KEY_X | KEY_A)) || (cpad.dy > 50);
        case HAL_KEY_B: return kDown & (KEY_B | KEY_Y);
        case HAL_KEY_LEFT: return (kDown & (KEY_LEFT | KEY_CPAD_LEFT));
        case HAL_KEY_RIGHT: return (kDown & (KEY_RIGHT | KEY_CPAD_RIGHT));
        default: return false;
    }
}

bool hal_key_held(Key key) {
    u32 kHeld = hidKeysHeld();
    circlePosition cpad;
    hidCircleRead(&cpad);
    switch(key) {
        case HAL_KEY_LEFT: return (kHeld & (KEY_LEFT | KEY_CPAD_LEFT)) || (cpad.dx < -50);
        case HAL_KEY_RIGHT: return (kHeld & (KEY_RIGHT | KEY_CPAD_RIGHT)) || (cpad.dx > 50);
        case HAL_KEY_A: return kHeld & (KEY_A | KEY_X);
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
