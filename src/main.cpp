#define WEBGPU_CPP_IMPLEMENTATION
#include "webgpu/webgpu.hpp"

#include "game/game.h"
#include "game/utility/timer.h"

auto main() -> int {
    Game game;
    FrameTimer timer(60);

    while (game.running()) {
        timer.update();

        game.input();
        game.update(timer.delta());
        game.render();

        timer.wait();
    }
}
