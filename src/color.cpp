#include <FastLED.h>
#include <math.h>

CRGB hue_to_rgb(float h, float s, float l) {
  float c = (1.0 - abs(2.0 * l - 1.0)) * s;
  float x = c * (1.0 - abs(fmod((h / 60.0), 2.0) - 1.0));
  float m = l - c / 2.0;

  CRGB val;
  if (h < 60.0) {
    val = CRGB((c + m) * 255.0, (x + m) * 255.0, 0.0);
  } else if (h < 120.0) {
    val = CRGB((x + m) * 255.0, (c + m) * 255.0, 0.0);
  } else if (h < 180.0) {
    val = CRGB(0.0, (c + m) * 255.0, (x + m) * 255.0);
  } else if (h < 240.0) {
    val = CRGB(0.0, (x + m) * 255.0, (c + m) * 255.0);
  } else if (h < 300.0) {
    val = CRGB((x + m) * 255.0, 0.0, (c + m) * 255.0);
  } else {
    val = CRGB((c + m) * 255.0, 0.0, (x + m) * 255.0);
  };

  return val;
}
