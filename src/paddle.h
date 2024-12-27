#ifndef PADDLE_H
#define PADDLE_H

#include <FastLED.h>

#include "const.h"
#include "structs.h"

void displayPaddle(CRGB leds[NUM_LEDS], Paddle &paddle);
void paddleDown(Paddle &paddle);
void paddleUp(Paddle &paddle);
void updatePaddle(Paddle &paddle);
void updatePaddleSmart(Ball &ball, Paddle &paddle);

#endif
