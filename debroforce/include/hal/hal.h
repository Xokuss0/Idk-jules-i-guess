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
    HAL_KEY_A, HAL_KEY_B, HAL_KEY_X, HAL_KEY_Y,
    HAL_KEY_L, HAL_KEY_R, HAL_KEY_ZL, HAL_KEY_ZR,
    HAL_KEY_START, HAL_KEY_SELECT,
    HAL_KEY_UP, HAL_KEY_DOWN, HAL_KEY_LEFT, HAL_KEY_RIGHT,
    HAL_KEY_CPAD_UP, HAL_KEY_CPAD_DOWN, HAL_KEY_CPAD_LEFT, HAL_KEY_CPAD_RIGHT
} Key;

void hal_init(void);
void hal_exit(void);
bool hal_is_running(void);

void hal_start_frame(void);
void hal_end_frame(void);

void hal_select_screen(bool top_screen);
void hal_clear_screen(unsigned char r, unsigned char g, unsigned char b);

void hal_set_camera(int x, int y);
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
