#ifndef HAL_H
#define HAL_H

#include <stdint.h>
#include <stdbool.h>

// Screen dimensions
#define TOP_WIDTH 400
#define TOP_HEIGHT 240
#define BOTTOM_WIDTH 320
#define BOTTOM_HEIGHT 240

// Colors (RGBA8)
typedef uint32_t Color;
#define COLOR_WHITE  0xFFFFFFFF
#define COLOR_BLACK  0x000000FF
#define COLOR_RED    0xFF0000FF
#define COLOR_GREEN  0x00FF00FF
#define COLOR_BLUE   0x0000FFFF
#define COLOR_YELLOW 0xFFFF00FF

// Input keys
typedef enum {
    KEY_A, KEY_B, KEY_X, KEY_Y,
    KEY_L, KEY_R, KEY_ZL, KEY_ZR,
    KEY_START, KEY_SELECT,
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
    KEY_CPAD_UP, KEY_CPAD_DOWN, KEY_CPAD_LEFT, KEY_CPAD_RIGHT
} Key;

void hal_init(void);
void hal_exit(void);
bool hal_is_running(void);

void hal_start_frame(void);
void hal_end_frame(void);

void hal_select_screen(bool top_screen);
void hal_clear_screen(unsigned char r, unsigned char g, unsigned char b);

void hal_draw_rect(int x, int y, int w, int h, Color color);
void hal_draw_text(int x, int y, const char* text, Color color);

bool hal_key_pressed(Key key);
bool hal_key_held(Key key);

typedef struct {
    int x, y;
} TouchPos;

bool hal_touch_held(TouchPos* pos);

typedef void* Texture;
Texture hal_load_texture(const char* path);
void hal_draw_texture(Texture tex, int x, int y, int w, int h, bool flip_h);

#endif
