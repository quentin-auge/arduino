#pragma once
#include <Arduino.h>

#include "leds.h"

class LedController {
public:
  LedController();
  int update(bool shortPress, bool longPress, bool doubleClick, bool tripleClick,
             int potentiometerValue, int* blinkInterval, int* fadeInterval);
  bool mustBlink() const;
  void exitTuning();

private:
  bool _brightnessTuning;
  int  _brightness;

  bool _blink;
  bool _blinkTuning;

  bool _fadeTuning;
};
