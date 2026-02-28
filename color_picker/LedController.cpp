#include "LedController.h"

LedController::LedController()
  : _on(false), _brightnessTuning(false), _tunedBrightness(MAX_BRIGHTNESS / 2),
    _blink(false), _blinkInterval(0), _blinkSharpness(0),
    _blinkPhase(0), _blinkTime(0)
{}

int LedController::update(int potentiometerValue, int* tunedBrightness) {
  int brightness = 0;

  if (_on && !_blink) {
    if (_brightnessTuning) {
      *tunedBrightness = map(potentiometerValue, 0, 4095, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    }
    brightness = *tunedBrightness;
  }
  
  if (_on && _blink) {
    // Tune blinking

    _blinkInterval = map(4095 - potentiometerValue, 0, 4095, 200, 2000);
    _blinkSharpness = map(potentiometerValue, 0, 4095, 11, 20);
    if (_blinkSharpness > 18) {
      // Force full blink at end of potentiometer course
      _blinkSharpness = 128;
    }

    // Blink

    float speed = 1000.0 / _blinkInterval; // Frequency in Hz (cycles per second)
    float sharpness = _blinkSharpness / 10.;  // 1.0 = smooth sine, more = sharper

    unsigned long now = millis();
    float deltaTime = (now - _blinkTime) / 1000.0;
    _blinkTime = now;

    // 1. Update phase (fixed frequency)
    _blinkPhase += 2.0 * PI * speed * deltaTime;
    if (_blinkPhase > 2.0 * PI) _blinkPhase -= 2.0 * PI;

    // 2. The "Stretched & Clamped" Formula
    // We center the sine at 0 (-1 to 1), multiply by sharpness,
    // then shift it back and constrain it.
    float rawSine = sin(_blinkPhase);
    float stretched = rawSine * sharpness;
    
    // Constrain to -1.0 to 1.0, then map to 0-1
    float clamped = constrain(stretched, -1.0, 1.0);
    float fadedBlinkPhase = (clamped + 1.0) / 2;

    // 4. Apply
    brightness = *tunedBrightness * fadedBlinkPhase;
  }

  return brightness;
}

void LedController::syncBlink(unsigned long blinkTime) {
  _blinkTime  = blinkTime;
  _blinkPhase = 0.0f;
}

bool LedController::isOn() const { return _on; }
bool LedController::isBlinking() const { return _blink; }

void LedController::turnOn() { _on = true; }
void LedController::turnOff() { _on = false; }
void LedController::toggleOnOff() { _on = !_on; }

void LedController::brightnessTuningOn() { _brightnessTuning = true; }
void LedController::brightnessTuningOff() { _brightnessTuning = false; }

void LedController::toggleBlinking() { _blink = !_blink; }
