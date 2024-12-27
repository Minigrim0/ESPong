#include <FastLED.h>
#include <cstdlib>
#include <ctime>

#define NUM_LEDS 8 * 32
#define DATA_PIN D1
#define PADDLE_LENGTH 3

bool randomPlay = true;
CRGB leds[NUM_LEDS];

unsigned int cycle;

int points[2] = {0, 0};

typedef struct paddle {
  int id;
  int y;
} Paddle;

typedef struct Ball {
  int x;
  int y;
  int dir_x;
  int dir_y;
} Ball;

Ball ball;
Paddle paddles[2] = {(Paddle){.id = 0, .y = 2}, (Paddle){.id = 1, .y = 2}};

void resetBall() {
  ball = {.x = 16,
          .y = 3,
          .dir_x = rand() % 2 == 0 ? 1 : -1,
          .dir_y = rand() % 2 == 0 ? 1 : -1};
}

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  cycle = 0;

  Serial.begin(115200);

  randomSeed(analogRead(A0) * 100);
  resetBall();
}

int CoordToIndex(int x, int y) {
  if (x % 2 == 1) {
    y = 7 - y;
  }
  return y + x * 8;
}

void displayPaddle(Paddle &paddle) {
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

void resetMatrix() {
  for (int x = 0; x < NUM_LEDS; x++) {
    leds[x] = CRGB::Black;
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
  resetBall();
  cycle = 0;
}

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

void displayBall(Ball &ball) {
  leds[CoordToIndex(ball.x, ball.y)] = CRGB(0, 10, 10);
}

void toggleGameType() { randomPlay = !randomPlay; }

void loop() {
    delay(50);
    cycle++;


    int res = Serial.read();
  if(!randomPlay) {
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
        resetBall();
        break;
    default:
        break;
    }
  } else {
      if (cycle % 3 == 0) {
        updatePaddle(paddles[0]);
        updatePaddle(paddles[1]);
      }
  }

  // Small delay before start
  if (cycle < 3) {
    return;
  }

  if (res == 'm') {
    toggleGameType();
  }

  updateBall(ball, paddles);
  if (ball.x < 0) {
    endGame(1);
  } else if (ball.x > 31) {
    endGame(0);
  }

  resetMatrix();

  displayPaddle(paddles[0]);
  displayPaddle(paddles[1]);

  displayBall(ball);

  FastLED.show();
}
