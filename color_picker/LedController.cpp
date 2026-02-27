#include "LedController.h"

LedController::LedController()
  : _brightnessTuning(false), _brightness(0),
    _blink(false), _blinkTuning(false), _fadeTuning(false)
{}

int LedController::update(bool shortPress, bool longPress, bool multiClick,
                          int potentiometerValue, int* blinkInterval, int* fadeSharpness) {
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

  // Handle long press when not blinking -> brightness tuning

  if (longPress && !_blink) {
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

  if (multiClick) {
    exitTuning();
    if (!_blink) {
      if (_brightness == 0) {
        // Blink right away
        _brightness = MAX_BRIGHTNESS;
      }
      _blink = true;
      _blinkTuning = true;
    } else {
      _blink = false;
      _brightness = 0;
    }
  }

  // Handle long press when blinking -> blink/fade tuning

  if (longPress && _blink) {
      if (!_blinkTuning) {
        exitTuning();
        _blinkTuning = true;
      } else {
        exitTuning();
        _fadeTuning = true;
      }
  }

  if (_blinkTuning) {
    // Set externally so it applies to all LEDs at once
    *blinkInterval = map(4095 - potentiometerValue, 0, 4095, 200, 2000);
  }

  if (_fadeTuning) {
    *fadeSharpness = map(potentiometerValue, 0, 4095, 8, 20);
    if (*fadeSharpness > 18) {
      // Force full blink at end of potentiometer course
      *fadeSharpness = 128;
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