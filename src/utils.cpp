#ifndef UTILS_H
#define UTILS_H

#include "utils.h"

int CoordToIndex(int x, int y) {
  if (x % 2 == 1) {
    y = 7 - y;
  }
  return y + x * 8;
}

#endif
