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

	void setDangerState(const Snake& snake) {
		Position rel_pos_head = get_rel_pos_vec(snake);
		if (rel_pos_head.y == 1 && rel_pos_head.x == 0) {
			states.danger_up = 1;
		} else if (rel_pos_head.y == 2 && rel_pos_head.x == 0) {
			states.danger_up_up = 1;
		} else if (rel_pos_head.y == 0 && rel_pos_head.x == 1) {
			states.danger_right = 1;
		} else if (rel_pos_head.y == 0 && rel_pos_head.x == 2) {
			states.danger_right_right = 1;
		} else if (rel_pos_head.y == 0 && rel_pos_head.x == -1) {
			states.danger_left = 1;
		} else if (rel_pos_head.y == 0 && rel_pos_head.x == -2) {
			states.danger_left_left = 1;
		}
	}

private:
	Position get_rel_pos_vec(const Snake& snake) {
		Position snake_movement_dir;
		switch (snake.dir) {
			// UP is normal case, no need for transformation.
			case UP: snake_movement_dir = {-1, 0}; return {snake.food.y - snake.pos[0].y, snake.food.x - snake.pos[0].x}; break;
		case DOWN: snake_movement_dir = {1, 0}; return {- (snake.food.y - snake.pos[0].y), -(snake.food.x - snake.pos[0].x)}; break;
		case RIGHT: snake_movement_dir = {0, 1}; return {-(snake.food.x - snake.pos[0].x), snake.food.y - snake.pos[0].y}; break;
		case LEFT: snake_movement_dir = {0, -1}; return {snake.food.x - snake.pos[0].x, -(snake.food.y - snake.pos[0].y)};  break;
			default: break;
		}
	
	}

};

#endif // AGENT_HPP
