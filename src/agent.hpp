#ifndef AGENT_HPP
#define AGENT_HPP

#include "snake.hpp"
#include <cmath>
#include <numbers>

template <typename T>
int signum(T val) {
    return (T(0) < val) - (val < T(0));
}

struct ActionSpace {
    Direction U;
};

struct StateSpace {
    int danger_left;
    int danger_left_left;
    int danger_right;
    int danger_right_right;
    int danger_up;
    int danger_up_up;
    float food_dist;
    float food_angle;
};

class Agent {
public:
    StateSpace states;
    ActionSpace actions;

    // void setFoodState(const Snake& snake) {
    //     int diff_x = snake.food.x - snake.pos[0].x;
    //     int diff_y = snake.food.y - snake.pos[0].y;
    //
    //     states.food_dist = std::sqrt(diff_x * diff_x + diff_y * diff_y);
    //
    //     float arccos_arg1 = snake.food.x * snake.pos[0].x + snake.food.y * snake.pos[0].y;
    //     float arccos_arg2 = snake.food.x * snake.pos[0].x * snake.food.y * snake.pos[0].y;
    //     float sign_fact = signum(snake.food.x * snake.pos[0].y - snake.food.y * snake.pos[0].x);
    //     states.food_angle = std::acos(arccos_arg1/arccos_arg2) * sign_fact;
    // }
	void setFoodState(const Snake& snake) {
		int diff_x = snake.food.x - snake.pos[0].x;
		int diff_y = snake.food.y - snake.pos[0].y;

		states.food_dist = std::sqrt(diff_x * diff_x + diff_y * diff_y);

		// Vector to food
		float fx = diff_x;
		float fy = diff_y;

		// Heading vector of the snake (head - next segment)
		float hx = snake.pos[0].x - snake.pos[1].x;
		float hy = snake.pos[0].y - snake.pos[1].y;

		// Normalize vectors
		float food_mag = std::sqrt(fx * fx + fy * fy);
		float head_mag = std::sqrt(hx * hx + hy * hy);

		if (food_mag == 0.0f || head_mag == 0.0f) {
			states.food_angle = 0.0f; // Prevent division by zero
			return;
		}

		// Dot and cross
		float dot = (hx * fx + hy * fy) / (head_mag * food_mag);
		float cross = hx * fy - hy * fx;

		// Clamp dot to [-1, 1] just in case
		dot = std::max(-1.0f, std::min(1.0f, dot));

		states.food_angle = (std::acos(dot) * signum(cross))/std::numbers::pi * 180;
	}
};

#endif // AGENT_HPP
