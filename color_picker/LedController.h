#pragma once
#include <Arduino.h>

#include "leds.h"

class LedController {
public:
  LedController();
  int update(bool shortPress, bool longPress, bool multiClick, int potentiometerValue);

  void exitTuning();

private:
  bool _brightnessTuning;
  int  _brightness;

  bool _blink;
  bool _blinkTuning;
  int  _blinkInterval;
  int  _blinkBrightness;
  unsigned long _blinkTime;
};
