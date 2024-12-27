#pragma once

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
