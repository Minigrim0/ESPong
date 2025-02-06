#include <FastLED.h>
#include <cstdlib>

#include "ball.h"
#include "color.h"
#include "const.h"
#include "paddle.h"

#include "utils.h"

Ball ball;
GameMode game_mode = RANDOM;
CRGB leds[NUM_LEDS];
int angle;

struct {
  int x;
  int y;
} previous_positions[AMOUNT_TRAIL];

unsigned int cycle;
unsigned char bounces = 0;

int points[2] = {0, 0};
Paddle paddles[2] = {(Paddle){.id = 0, .y = 2}, (Paddle){.id = 1, .y = 2}};

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  cycle = 0;
  angle = 0;

  Serial.begin(115200);

  randomSeed(analogRead(A0) * 100);
  resetBall(ball);
}

void resetMatrix() {
  for (int x = 0; x < NUM_LEDS; x++) {
    leds[x] = CRGB::Black;
  }
}

void resetPreviousPos() {
  for (int i = 0; i < AMOUNT_TRAIL; i++) {
    previous_positions[i] = {0, 0};
  }
}

void shiftPreviousPos() {
  for (int i = AMOUNT_TRAIL - 1; i >= 0; i--) {
    previous_positions[i] = previous_positions[i - 1];
  }
}

void displayPreviousPos() {
  for (int i = 0; i < AMOUNT_TRAIL; i++) {
    leds[CoordToIndex(previous_positions[i].x, previous_positions[i].y)] =
        hue_to_rgb(angle - i, 1, 0.5).fadeToBlackBy(5 * i);
  }
}

void endGame(int winner) {
  points[winner]++;
  for (int i = 0; i < 6; i++) {
    for (int x = 16 * winner; x < 16 * (winner + 1); x++) {
      for (int y = 0; y < 8; y++) {
        leds[CoordToIndex(x, y)] = i % 2 == 0 ? CRGB(0, 10, 0) : CRGB::Black;
      }
    }
    FastLED.show();
    delay(100);
  }
  resetBall(ball);
  cycle = 0;
}

void toggleGameType() {
  switch (game_mode) {
  case RANDOM:
    game_mode = MANUAL;
    break;
  case MANUAL:
    game_mode = SMART;
    break;
  case SMART:
    game_mode = RANDOM;
    break;
  default:
    game_mode = RANDOM;
    break;
  }
}

void readInput(unsigned char res) {
  switch (res) {
  case 'w':
    paddleUp(paddles[0]);
    break;
  case 's':
    paddleDown(paddles[0]);
    break;
  case 'i':
    paddleUp(paddles[1]);
    break;
  case 'k':
    paddleDown(paddles[1]);
    break;
  case 'r':
    resetBall(ball);
    break;
  default:
    break;
  }
}

void loop() {
  delay(50);
  cycle++;
  angle++;

  int res = Serial.read();
  switch (game_mode) {
  case MANUAL:
    readInput(res);
    break;
  case SMART:
    if (ball.dir_x > 0) {
      if (ball.x > 25) {
        updatePaddleSmart(ball, paddles[1]);
      } else {
        updatePaddle(paddles[1]);
      }
      updatePaddle(paddles[0]);
    } else {
      if (ball.x < 6) {
        updatePaddleSmart(ball, paddles[0]);
      } else {
        updatePaddle(paddles[0]);
      }
      updatePaddle(paddles[1]);
    }
    break;
  case RANDOM:
    if (cycle % 3 == 0) {
      updatePaddle(paddles[0]);
      updatePaddle(paddles[1]);
    }
    break;
  default:
    break;
  }

  // Small delay before start
  if (cycle < 3) {
    return;
  }

  if (res == 'm') {
    toggleGameType();
  }

  shiftPreviousPos();
  previous_positions[0] = {ball.x, ball.y};
  updateBall(ball, paddles);

  if (ball.x < 0) {
    endGame(1);
  } else if (ball.x > 31) {
    endGame(0);
  }

  resetMatrix();

  displayPaddle(leds, paddles[0]);
  displayPaddle(leds, paddles[1]);

  displayPreviousPos();
  displayBall(leds, ball, angle);

  FastLED.show();
}
