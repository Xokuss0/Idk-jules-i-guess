#include "hal/hal.h"
#include "game.h"
#include "assets.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    srand(time(NULL));
    hal_init();
    assets_init();

    Game game;
    game_init(&game);

    float dt = 1.0f / 60.0f;

    while (hal_is_running() && game.running) {
        game_handle_input(&game);
        game_update(&game, dt);
        game_render(&game);
    }

    assets_exit();
    hal_exit();
    return 0;
}
