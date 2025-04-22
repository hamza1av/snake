#pragma once
#include <deque>

struct Position {
    int y, x;
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake {
public:
    std::deque<Position> pos;
    Direction dir = LEFT;
    Position food;
    bool game_over = false;

    Snake();
    void get_user_input(int ch);
    void move();
    void gen_new_food_pos();
    Position setNewCoordinates(Position head, Direction dir);
};
