#ifndef __3DS__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <string.h>
#include "hal/hal.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static bool running = true;
static bool drawing_top = true;

void hal_init(void) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    window = SDL_CreateWindow("Debroforce POC (PC)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TOP_WIDTH, TOP_HEIGHT + BOTTOM_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void hal_exit(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool hal_is_running(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
    }
    return running;
}

void hal_start_frame(void) {
}

void hal_end_frame(void) {
    SDL_RenderPresent(renderer);
}

void hal_select_screen(bool top_screen) {
    drawing_top = top_screen;
}

void hal_clear_screen(unsigned char r, unsigned char g, unsigned char b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_Rect rect = {0, drawing_top ? 0 : TOP_HEIGHT, TOP_WIDTH, drawing_top ? TOP_HEIGHT : BOTTOM_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);
}

void hal_draw_rect(int x, int y, int w, int h, Color color) {
    SDL_SetRenderDrawColor(renderer, (color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    int offset_y = drawing_top ? 0 : TOP_HEIGHT;
    SDL_Rect r = {x, y + offset_y, w, h};
    SDL_RenderFillRect(renderer, &r);
}

void hal_draw_text(int x, int y, const char* text, Color color) {
}

Texture hal_load_texture(const char* path) {
    return (Texture)IMG_LoadTexture(renderer, path);
}

void hal_draw_texture(Texture tex, int x, int y, int w, int h, bool flip_h) {
    if (!tex) return;
    int offset_y = drawing_top ? 0 : TOP_HEIGHT;
    SDL_Rect dst = {x, y + offset_y, w, h};
    SDL_RenderCopyEx(renderer, (SDL_Texture*)tex, NULL, &dst, 0, NULL, flip_h ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

bool hal_key_pressed(Key key) {
    static uint8_t last_state[SDL_NUM_SCANCODES];
    const uint8_t* state = SDL_GetKeyboardState(NULL);

    bool pressed = false;
    switch(key) {
        case HAL_KEY_START: pressed = state[SDL_SCANCODE_RETURN] && !last_state[SDL_SCANCODE_RETURN]; break;
        case HAL_KEY_SELECT: pressed = state[SDL_SCANCODE_RSHIFT] && !last_state[SDL_SCANCODE_RSHIFT]; break;
        case HAL_KEY_UP: pressed = (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) && !(last_state[SDL_SCANCODE_UP] || last_state[SDL_SCANCODE_W]); break;
        case HAL_KEY_B: pressed = state[SDL_SCANCODE_X] && !last_state[SDL_SCANCODE_X]; break;
        default: break;
    }

    memcpy(last_state, state, SDL_NUM_SCANCODES);
    return pressed;
}

bool hal_key_held(Key key) {
    const uint8_t* state = SDL_GetKeyboardState(NULL);
    switch(key) {
        case HAL_KEY_LEFT: return state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A];
        case HAL_KEY_RIGHT: return state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D];
        case HAL_KEY_A: return state[SDL_SCANCODE_Z];
        default: return false;
    }
}

bool hal_touch_held(TouchPos* pos) {
    int mx, my;
    uint32_t state = SDL_GetMouseState(&mx, &my);
    if (state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        pos->x = mx;
        pos->y = my;
        return true;
    }
    return false;
}
#endif
