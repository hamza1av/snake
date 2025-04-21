
#include <algorithm>
#include <ncurses.h>
#include <unistd.h>   // usleep
#include <vector>
#include <random>
#include <iostream>

constexpr int HEIGHT = 30;
constexpr int WIDTH = 30;

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
	std::uniform_int_distribution<> dis_x(0 , WIDTH);
	std::uniform_int_distribution<> dis_y(0 , HEIGHT);
	int rand_x = dis_x(gen);
	int rand_y = dis_y(gen);
	return {rand_x, rand_y};
}


class Snake {
	public: 
	std::deque<Position> pos;
	Direction dir = LEFT;
	std::string body_down = "";
	std::string body_up = "";
	std::string body_left = "";
	std::string body_right = "";
	std::string body_head = "";
	std::string body_tail = "";

	Snake() {
		// spawn first element of snake
		// pos.push_back(get_rand_pos());
	}

	void move() {
		Position head = pos.front();
		Position new_head;

		switch (dir) {
			case UP:
				new_head = {head.y - 1, head.x};
				break;
			case DOWN:
				new_head = {head.y + 1, head.x};
				break;
			case RIGHT:
				new_head = {head.y, head.x + 1};
				break;
			case LEFT:
				new_head = {head.y, head.x - 1};
				break;
		}

		pos.push_front(new_head);    // Add new head
		pos.pop_back();            // Remove tail
	}

	void get_user_input(int ch) {
		switch (ch) {
			case KEY_UP:
			dir = UP;
			break;
			case KEY_DOWN:
			dir = DOWN;
			break;
			case KEY_RIGHT:
			dir = RIGHT;
			break;
			case KEY_LEFT:
			dir = LEFT;
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

    // Snake body as vector of positions
	Snake snake;
    snake.pos = {
        {10, 10}, {10, 11}, {10, 12}, {10, 13}, {10, 14}
    };

    // Food position
    Position food = {5, 20};

    while (true) {

        // Check for quit key
        int ch = getch();
        if (ch == 'q') break;

		snake.get_user_input(ch);
		snake.move();

        // clear();
		erase();
        box(stdscr, 0, 0);  // Draw border

        // Draw snake
        attron(COLOR_PAIR(1));
		bool first = true;
		for (const auto& segment : snake.pos) {
			if (first) {
				mvaddch(segment.y, segment.x, '@');
				first = false;
			} else {
				mvaddch(segment.y, segment.x, '0');
			}
		}
        attroff(COLOR_PAIR(1));

        // Draw food
        attron(COLOR_PAIR(2));
        mvaddch(food.y, food.x, 'X');
        attroff(COLOR_PAIR(2));

        attron(COLOR_PAIR(2));
        mvaddch(1, 1, '1');
        mvaddch(2, 2, '2');
        mvaddch(4, 4, '4');
        attroff(COLOR_PAIR(2));


        refresh();


        usleep(100000); // 100ms delay
    }

    endwin();
    return 0;
}
