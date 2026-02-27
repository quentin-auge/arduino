#include "Button.h"

Button::Button(int pin)
  : _pin(pin),
    _lastRawState(HIGH), _currentState(HIGH), _previousState(HIGH),
    _pressedTime(0), _releasedTime(0),
    _lastDebounceTime(0), _lastReleaseTime(0),
    _pressed(false), _shortPress(false), _longPress(false),
    _clickCount(0), _doubleClick(false), _tripleClick(false), _multiClick(false)
 {}

void Button::update() {
  _pressed     = false;
  _shortPress  = false;
  _longPress   = false;
  _doubleClick = false;
  _tripleClick = false;
  _multiClick  = false;

  int rawState = digitalRead(_pin);

  // Reset debounce timer on any change
  if (rawState != _lastRawState) {
    _lastDebounceTime = millis();
  }

  if (millis() - _lastDebounceTime >= DEBOUNCE_MS) {
    _currentState = rawState;

    // Press (HIGH -> LOW)
    if (_previousState == HIGH && _currentState == LOW) {
      _pressedTime = millis();
      _pressed = true;
    }

    // Release (LOW -> HIGH)
    if (_previousState == LOW && _currentState == HIGH) {
      _releasedTime = millis();

      if (_releasedTime - _pressedTime >= LONG_PRESS_MS) {
        // Long press — fire immediately, reset click count
        _longPress  = true;
        _clickCount = 0;
      } else {
        // Short click — increment and record when it happened
        _clickCount++;
        _lastReleaseTime = _releasedTime;
      }
    }

    _previousState = _currentState;
  }

  _lastRawState = rawState;

  // Multi-click resolution: once the multi-click window expires with no new click, fire the verdict
  if (_clickCount > 0 && _currentState == HIGH &&
      millis() - _lastReleaseTime >= MULTI_CLICK_MS) {

    if (_clickCount == 1)
      _shortPress  = true;

    if (_clickCount > 1)
      _multiClick  = true;

    if (_clickCount == 2)
      _doubleClick = true;

    if (_clickCount == 3)
      _tripleClick = true;

    _clickCount = 0;
  }
}

bool Button::isPressed()     const { return _pressed; }
bool Button::isShortPress()  const { return _shortPress; }
bool Button::isLongPress()   const { return _longPress; }
bool Button::isDoubleClick() const { return _doubleClick; }
bool Button::isTripleClick() const { return _tripleClick; }
bool Button::isMultiClick()  const { return _multiClick; }
bool Button::isReleased()    const { return _shortPress || _longPress || _multiClick; }
