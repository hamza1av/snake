#include "snake.hpp"
#include <cmath>

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
};
