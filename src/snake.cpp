#include "snake.hpp"
#include <random>
#include <algorithm>
#include <ncurses.h>

constexpr int GRID_HEIGHT = 20;
constexpr int GRID_WIDTH = 40;

Position get_rand_pos() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_y(1 , GRID_WIDTH - 1);
    std::uniform_int_distribution<> dis_x(1 , GRID_HEIGHT - 1);
    return {dis_x(gen), dis_y(gen)};
}

Snake::Snake() {
    pos.push_back(get_rand_pos());
    gen_new_food_pos();
}

void Snake::gen_new_food_pos() {
    bool needs_new_trial = true;
    while (needs_new_trial) {
        food = get_rand_pos();
        needs_new_trial = false;
        for (auto& body_part : pos) {
            if (food == body_part) {
                needs_new_trial = true;
                break;
            }
        }
    }
}

Position Snake::setNewCoordinates(Position head, Direction dir) {
    Position new_head = head;
    switch (dir) {
        case UP:    new_head.y = (head.y - 1 <= 0) ? GRID_HEIGHT - 1 : head.y - 1; break;
        case DOWN:  new_head.y = (head.y + 1 >= GRID_HEIGHT) ? 1 : head.y + 1; break;
        case LEFT:  new_head.x = (head.x - 1 <= 0) ? GRID_WIDTH - 1 : head.x - 1; break;
        case RIGHT: new_head.x = (head.x + 1 >= GRID_WIDTH) ? 1 : head.x + 1; break;
    }
    return new_head;
}

void Snake::move() {
    Position head = pos.front();
    Position new_head = setNewCoordinates(head, dir);

    if (std::find(pos.begin(), pos.end(), new_head) != pos.end()) {
        game_over = true;
        return;
    }

    pos.push_front(new_head);
    if (new_head == food)
        gen_new_food_pos();
    else
        pos.pop_back();
}

void Snake::get_user_input(int ch) {
    switch (ch) {
        case KEY_UP:    if (dir != DOWN) dir = UP; break;
        case KEY_DOWN:  if (dir != UP) dir = DOWN; break;
        case KEY_RIGHT: if (dir != LEFT) dir = RIGHT; break;
        case KEY_LEFT:  if (dir != RIGHT) dir = LEFT; break;
    }
}
