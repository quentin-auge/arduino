#include "LedController.h"

LedController::LedController()
  : _brightnessTuning(false), _brightness(0),
    _blink(false), _blinkTuning(false), _fadeTuning(false)
{}

int LedController::update(bool shortPress, bool longPress, bool doubleClick, bool tripleClick,
                          int potentiometerValue, int* blinkInterval, int* fadeInterval) {
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
    if (!_brightnessTuning) {
      exitTuning();
      _blink = false;
      _brightnessTuning = true;
    } else {
      exitTuning();
      _brightness = 0;
    }
  }

  if (_brightnessTuning) {
    _brightness = map(potentiometerValue, 0, 4095, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  }

  // Handle multi-click -> blink

  if (doubleClick || tripleClick) {
    exitTuning();
    if (!_blink) {
      if (_brightness == 0) {
        // Blink right away
        _brightness = MAX_BRIGHTNESS;
      }
      _blink = true;
    }
  }

  // Handle double click -> blink tuning

  if (doubleClick) {
      exitTuning();
      _blinkTuning = true;
  }

  if (_blinkTuning) {
    // Set externally so it applies to all LEDs at once
    *blinkInterval = map(4095 - potentiometerValue, 0, 4095, 200, 2000);
  }

  // Handle triple click -> blink tuning
  if (tripleClick) {
      exitTuning();
      _fadeTuning = true;
  }

  if (_fadeTuning) {
    *fadeInterval = map(4095 - potentiometerValue, 0, 4095, 8, 18);
    if (*fadeInterval > 17) {
      *fadeInterval = 64;
    }
  }

  return _brightness;
}

bool LedController::mustBlink() const { return _blink; }

void LedController::exitTuning() {
  _brightnessTuning = false;
  _blinkTuning      = false;
  _fadeTuning       = false;
}