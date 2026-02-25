#pragma once

#include <Arduino.h>

#define DEBOUNCE_MS    20
#define LONG_PRESS_MS  500
#define MULTI_CLICK_MS 250

class Button {
public:
  Button(int pin);

  void update();

  bool isPressed()     const;
  bool isShortPress()  const;
  bool isLongPress()   const;
  bool isDoubleClick() const;
  bool isMultiClick()  const;
  bool isReleased()    const;

private:
  int  _pin;

  int  _lastRawState;
  unsigned long _lastDebounceTime;

  int  _currentState;
  int  _previousState;

  unsigned long _pressedTime;
  unsigned long _releasedTime;

  unsigned long _lastReleaseTime;
  int  _clickCount;

  bool _pressed;
  bool _shortPress;
  bool _longPress;
  bool _doubleClick;
  bool _multiClick;
};