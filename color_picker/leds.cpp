#include "leds.h"
#include "pins.h"
#include "pitches.h"

void setRGBFromPitch(int pitch) {
  if (pitch == REST) {
    analogWrite(LED_R_PIN, 0);
    analogWrite(LED_G_PIN, 0);
    analogWrite(LED_B_PIN, 0);
    return;
  }

  // Constrain the range to avoid glitchy colors
  // 131Hz (C3) to 1047Hz (C6) covers most melodies
  int hue = map(constrain(pitch, 131, 1047), 131, 1047, 0, MAX_BRIGHTNESS);
  
  // Hue -> RGB
  int r, g, b;
  if (hue < 85) {
    r = MAX_BRIGHTNESS - hue * 3; g = hue * 3; b = 0;
  } else if (hue < 170) {
    hue -= 85;
    r = 0; g = MAX_BRIGHTNESS - hue * 3; b = hue * 3;
  } else {
    hue -= 170;
    r = hue * 3; g = 0; b = MAX_BRIGHTNESS - hue * 3;
  }

  analogWrite(LED_R_PIN, r);
  analogWrite(LED_G_PIN, g);
  analogWrite(LED_B_PIN, b);
}