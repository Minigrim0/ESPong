#pragma once

#include <FastLED.h>
#include "structs.h"

void updateBall(Ball &ball, Paddle paddles[2]);
void displayBall(CRGB* leds, Ball &ball, int angle);
void resetBall(Ball &ball);
