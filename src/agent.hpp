#pragma once
#include "snake.hpp"

class Agent {
public:
    Direction choose_action(const Snake& snake);
    void update_policy(); // for training later
};
