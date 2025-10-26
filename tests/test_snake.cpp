#include <gtest/gtest.h>
#include "snake.hpp"

TEST(SnakeTest, InitialPosition) {
    Snake snake({10, 10}, 4);
    ASSERT_EQ(snake.pos.size(), 4);
    EXPECT_EQ(snake.pos[0].y, 10);
    EXPECT_EQ(snake.pos[0].x, 10);
}

TEST(SnakeTest, Move) {
    Snake snake({10, 10}, 4);
    snake.dir = UP;
    snake.move();
    EXPECT_EQ(snake.pos[0].y, 9);
    EXPECT_EQ(snake.pos[0].x, 10);
}
