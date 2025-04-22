#include <deque>
#include <ncurses.h>
#include <unistd.h>   // usleep
#include <random>
#include <algorithm>

#include "snake.hpp"
#include "agent.hpp"

constexpr int GRID_HEIGHT = 20;
constexpr int GRID_WIDTH = 40;
constexpr int GRID_START_Y = 4;
constexpr int GRID_START_X = 2;




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

    while (!snake.game_over) {

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

        usleep(100000); // 100ms delay
		refresh();
    }

    endwin();
	    // Game over screen
    nodelay(stdscr, FALSE); // Wait for key press
    clear();
    mvaddstr(GRID_HEIGHT / 2, (GRID_WIDTH / 2) - 5, "Game Over!");
    mvaddstr((GRID_HEIGHT / 2) + 1, (GRID_WIDTH / 2) - 12, "Press 'q' to quit or 'r' to restart");
    refresh();

    int ch;
    while ((ch = getch()) != 'q') {
        if (ch == 'r') {
            // Optional: handle restart logic here
            break;
        }
    }


    endwin();
    return 0;
}
