#include "snake.h"

auto Snake::get_direction() -> SnakeDirection {
    return m_direction;
}

auto Snake::set_direction(SnakeDirection direction) -> void {
    m_direction = direction;
}
