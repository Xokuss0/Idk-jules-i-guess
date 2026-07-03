#ifndef __3DS__
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "hal/hal.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static bool running = true;

void hal_init(void) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Debroforce POC (PC)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TOP_WIDTH, TOP_HEIGHT + BOTTOM_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void hal_exit(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void hal_end_frame(void) {
    SDL_RenderPresent(renderer);
}

void hal_draw_rect(int x, int y, int w, int h, Color color) {
    SDL_SetRenderDrawColor(renderer, (color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    SDL_Rect r = {x, y, w, h};
    SDL_RenderFillRect(renderer, &r);
}

void hal_draw_text(int x, int y, const char* text, Color color) {
}

bool hal_key_pressed(Key key) {
    static uint8_t last_state[SDL_NUM_SCANCODES];
    const uint8_t* state = SDL_GetKeyboardState(NULL);

    bool pressed = false;
    switch(key) {
        case KEY_START: pressed = state[SDL_SCANCODE_RETURN] && !last_state[SDL_SCANCODE_RETURN]; break;
        case KEY_SELECT: pressed = state[SDL_SCANCODE_RSHIFT] && !last_state[SDL_SCANCODE_RSHIFT]; break;
        case KEY_UP: pressed = (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) && !(last_state[SDL_SCANCODE_UP] || last_state[SDL_SCANCODE_W]); break;
        case KEY_B: pressed = state[SDL_SCANCODE_X] && !last_state[SDL_SCANCODE_X]; break; // Special
        default: break;
    }

    // This isn't perfect for all keys since it's a stub, but enough for testing
    memcpy(last_state, state, SDL_NUM_SCANCODES);
    return pressed;
}

bool hal_key_held(Key key) {
    const uint8_t* state = SDL_GetKeyboardState(NULL);
    switch(key) {
        case KEY_LEFT: return state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A];
        case KEY_RIGHT: return state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D];
        case KEY_A: return state[SDL_SCANCODE_Z]; // Attack
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
