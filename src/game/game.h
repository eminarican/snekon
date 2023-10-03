#ifndef SNEKON_GAME_H
#define SNEKON_GAME_H

#include "window/window.h"
#include "window/renderer.h"
#include "snake.h"

class Game {
public:
    Game();

    auto running() -> bool;

    auto input() -> void;
    auto update(double time_delta) -> void;
    auto render() -> void;

private:
    Window m_window {};
    Renderer m_renderer {};

    Snake m_snake {};
};

#endif //SNEKON_GAME_H
