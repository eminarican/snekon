#include "game.h"
#include "utility/utility.h"

Game::Game() {
    unwrap_expected_(m_window, Window::init())
    unwrap_expected_(m_renderer, Renderer::init(m_window.handle()))
}

auto Game::running() -> bool {
    return !m_window.closed();
}

auto Game::input() -> void {
    m_window.update();

    if (m_window.key(KeyCode::Up) == KeyState::Press || m_window.key(KeyCode::W) == KeyState::Press) {
        m_snake.set_direction(SnakeDirection::Up);
    }
    if (m_window.key(KeyCode::Down) == KeyState::Press || m_window.key(KeyCode::S) == KeyState::Press) {
        m_snake.set_direction(SnakeDirection::Down);
    }
    if (m_window.key(KeyCode::Right) == KeyState::Press || m_window.key(KeyCode::D) == KeyState::Press) {
        m_snake.set_direction(SnakeDirection::Right);
    }
    if (m_window.key(KeyCode::Left) == KeyState::Press || m_window.key(KeyCode::A) == KeyState::Press) {
        m_snake.set_direction(SnakeDirection::Left);
    }
}

auto Game::update(double time_delta) -> void {
}

auto Game::render() -> void {
    m_renderer.render(true);
}
