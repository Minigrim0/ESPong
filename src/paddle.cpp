#include "paddle.h"
#include "const.h"
#include "utils.h"

#include <cstdlib>

void displayPaddle(CRGB leds[NUM_LEDS], Paddle &paddle) {
  for (int x = paddle.y; x < paddle.y + PADDLE_LENGTH; x++) {
    leds[CoordToIndex(paddle.id * 31, x)] = CRGB(10, 0, 0);
  }
}

void paddleDown(Paddle &paddle) {
  if (paddle.y < 8 - PADDLE_LENGTH) {
    paddle.y++;
  }
}

void paddleUp(Paddle &paddle) {
  if (paddle.y > 0) {
    paddle.y--;
  }
}

void updatePaddle(Paddle &paddle) {
  switch (std::rand() % 3) {
  case 0:
    paddleUp(paddle);
    break;
  case 1:
    paddleDown(paddle);
    break;
  default:
    break;
  }
}

void updatePaddleSmart(Ball &ball, Paddle &paddle) {
    if ((paddle.y + 1) < ball.y) {
        paddleDown(paddle);
    } else if ((paddle.y + 1) > ball.y) {
        paddleUp(paddle);
    }
}
