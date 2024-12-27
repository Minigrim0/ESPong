#include "ball.h"
#include "color.h"
#include "const.h"
#include "utils.h"

void updateBall(Ball &ball, Paddle paddles[2]) {
  if (ball.dir_y == 1 && ball.y == 7) {
    ball.dir_y = -1;
  } else if (ball.dir_y == -1 && ball.y == 0) {
    ball.dir_y = 1;
  }

  if (ball.x == 30 && ball.dir_x == 1 &&
      ball.y <= paddles[1].y + PADDLE_LENGTH && ball.y >= paddles[1].y) {
    ball.dir_x = -1;
  }
  if (ball.x == 1 && ball.dir_x == -1 &&
      ball.y <= paddles[0].y + PADDLE_LENGTH && ball.y >= paddles[0].y) {
    ball.dir_x = 1;
  }

  ball.x += ball.dir_x;
  ball.y += ball.dir_y;
}

void displayBall(CRGB* leds, Ball &ball, int angle) {
  leds[CoordToIndex(ball.x, ball.y)] = hue_to_rgb(angle, 1, 0.5);
}

void resetBall(Ball &ball) {
  ball = {.x = 16,
          .y = 3,
          .dir_x = rand() % 2 == 0 ? 1 : -1,
          .dir_y = rand() % 2 == 0 ? 1 : -1};
}
