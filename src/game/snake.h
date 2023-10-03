#ifndef SNEKON_SNAKE_H
#define SNEKON_SNAKE_H

enum class SnakeDirection {
    Up, Down, Right, Left,
};

class Snake {
public:
    auto get_direction() -> SnakeDirection;
    auto set_direction(SnakeDirection direction) -> void ;

private:
    SnakeDirection m_direction;
};

#endif //SNEKON_SNAKE_H
