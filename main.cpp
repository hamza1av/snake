#include <deque>
#include <ncurses.h>
#include <unistd.h>   // usleep
#include <random>

constexpr int GRID_HEIGHT = 20;
constexpr int GRID_WIDTH = 40;
constexpr int GRID_START_Y = 4;
constexpr int GRID_START_X = 2;

struct Position {
    int y, x;
};

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

Position get_rand_pos() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis_y(1 , GRID_WIDTH-1);
	std::uniform_int_distribution<> dis_x(1 , GRID_HEIGHT-1);
	int rand_x = dis_x(gen);
	int rand_y = dis_y(gen);
	return {rand_x, rand_y};
}

class Snake {
public: 
	std::deque<Position> pos;
	Direction dir = LEFT;
	Position food;
	void get_new_food_pos();

	Snake() {
		// spawn first element of snake
		pos.push_back(get_rand_pos());
		gen_new_food_pos();

	}

	void gen_new_food_pos() {
		bool needs_new_trial = true;
		while (needs_new_trial) {
			food = get_rand_pos();
			for (auto& body_part : pos) {
				if (food.x == body_part.x || food.y == body_part.y) {
					break;
				}
			}
			needs_new_trial = false;
		}
	}

	Position setNewCoordinates(Position head, Direction dir) {
		Position new_head;
		switch (dir) {
			case UP:
				new_head = {head.y - 1, head.x};
				if (new_head.y <= 0) {
					new_head.y = GRID_HEIGHT - 1;
				}
				break;
			case DOWN:
				new_head = {head.y + 1, head.x};
				if (new_head.y >= GRID_HEIGHT) {
					new_head.y = 1;
				}
				break;
			case RIGHT:
				new_head = {head.y, head.x + 1};
				if (new_head.x >= GRID_WIDTH) {
					new_head.x = 1;
				}
				break;
			case LEFT:
				new_head = {head.y, head.x - 1};
				if (new_head.x <= 0) {
					new_head.x = GRID_WIDTH - 1;
				}
				break;
		}
		return new_head;
	}

	void move() {
		Position head = pos.front();
		Position new_head;

		new_head = setNewCoordinates(head, dir);

		pos.push_front(new_head);    // Add new head
		
		if (!(head.x == food.x && head.y == food.y)) {
			pos.pop_back();            // Remove tail
		} else {
			gen_new_food_pos();
		}
	}

	void get_user_input(int ch) {
		switch (ch) {
			case KEY_UP:
			if (dir!= DOWN) dir = UP;
			break;
			case KEY_DOWN:
			if (dir!=UP) dir = DOWN;
			break;
			case KEY_RIGHT:
			if (dir!= LEFT) dir = RIGHT;
			break;
			case KEY_LEFT:
			if (dir!= RIGHT) dir = LEFT;
			break;
			default:
			break;
		}
	}
};

int main() {
    // Init ncurses
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // Optional color support
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK); // Snake color
        init_pair(2, COLOR_RED, COLOR_BLACK);   // Food color
    }

	WINDOW* game_win = newwin(GRID_HEIGHT + 2, GRID_WIDTH + 2, GRID_START_Y, GRID_START_X);

	Snake snake;
    // snake.pos = {
    //     {10, 10}, {10, 11}, {10, 12}, {10, 13}, {10, 14}
    // };
	snake.food = {14,14};

	mvaddstr(0, 0, "Snake Game - Press 'q' to quit");
	// refresh();

    while (true) {

		std::string head_position_text = "Head Position: X=" + std::to_string(snake.pos[0].x) + " Y=" + std::to_string(snake.pos[0].y);
		std::string food_position_text = "Food Position: X=" + std::to_string(snake.food.x) + " Y=" + std::to_string(snake.food.y);
		erase();
		mvaddstr(1, 2, head_position_text.c_str());
		mvaddstr(2, 2, food_position_text.c_str());
        // Check for quit key
        int ch = getch();
        if (ch == 'q') break;

		snake.get_user_input(ch);
		snake.move();

        // clear();
		werase(game_win);
        box(game_win, 0, 0);  // Draw border

        // Draw snake
        attron(COLOR_PAIR(1));
		bool first = true;
		for (const auto& segment : snake.pos) {
			if (first) {
				mvwaddch(game_win, segment.y, segment.x, '@');
				first = false;
			} else {
				mvwaddch(game_win, segment.y, segment.x, '0');
			}
		}
        attroff(COLOR_PAIR(1));

        // Draw food
        attron(COLOR_PAIR(2));
        mvwaddch(game_win, snake.food.y, snake.food.x, 'X');
        attroff(COLOR_PAIR(2));



        wrefresh(game_win);

        usleep(200000); // 100ms delay
		refresh();
    }

    endwin();
    return 0;
}
