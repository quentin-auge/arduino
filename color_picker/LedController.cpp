#include "LedController.h"

LedController::LedController()
  : _brightnessTuning(false), _brightness(0),
    _blink(false), _blinkTuning(false), _blinkInterval(500),
    _blinkBrightness(0), _blinkTime(0)
{}

int LedController::update(bool shortPress, bool longPress, bool multiClick, int potentiometerValue) {
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
      _blinkBrightness = _brightness;
      _blinkTuning = true;
      _blinkTime = millis();
      _blink = true;
    } else {
      // Stop blinking, keep same brightness
      _blink = false;
      _brightness = _blinkBrightness;
    }
  }

  if (_blinkTuning) {
    _blinkInterval = map(4095 - potentiometerValue, 0, 4095, 100, 1000);
  }

  if (_blink && millis() - _blinkTime > _blinkInterval) {
    _brightness = (_brightness == 0) ? _blinkBrightness : 0;
    _blinkTime = millis();
  }

  return _brightness;
}

void LedController::exitTuning() {
  _brightnessTuning = false;
  _blinkTuning      = false;
}