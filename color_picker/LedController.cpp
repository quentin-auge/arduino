#include "LedController.h"

LedController::LedController()
  : _brightnessTuning(false), _brightness(0),
    _blink(false), _blinkTuning(false)
{}

int LedController::update(bool shortPress, bool longPress, bool multiClick, int potentiometerValue, int* blinkInterval) {
  // Handle short press -> on/off

  if (shortPress) {
    exitTuning();
    if (!_blink) {
      // Switch on/off
      _brightness = _brightness != MAX_BRIGHTNESS ? MAX_BRIGHTNESS : 0;
    } else {
      _blink = false;
      _brightness = MAX_BRIGHTNESS;
    }
  }

  // Handle long press -> brightness tuning

  if (longPress) {
    exitTuning();
    _blink = false;
    _brightnessTuning = true;
  }

  if (_brightnessTuning) {
    _brightness = map(potentiometerValue, 0, 4095, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  }

  // Handle multi-click -> blink tuning

  if (multiClick) {
    exitTuning();
    if (!_blink) {
      if (_brightness == 0) {
        // Blink right away
        _brightness = MAX_BRIGHTNESS;
      }
      _blinkTuning = true;
      _blink = true;
    } else {
      _blink = false;
      _brightness = 0;
    }
  }

  if (_blinkTuning) {
    // Set externally so it applies to all LEDs at once
    *blinkInterval = map(4095 - potentiometerValue, 0, 4095, 100, 1000);
  }

  return _brightness;
}

bool LedController::mustBlink() const { return _blink; }

void LedController::exitTuning() {
  _brightnessTuning = false;
  _blinkTuning      = false;
}