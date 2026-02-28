#pragma once
#include <Arduino.h>

#include "leds.h"

class LedController {
public:
  LedController();
  int update(int potentiometerValue, int* tunedBrightness);

  void syncBlink(unsigned long t);

  bool isOn() const;
  bool isBlinking() const;

  void turnOn();
  void turnOff();
  void toggleOnOff();

  void brightnessTuningOn();
  void brightnessTuningOff();

  void toggleBlinking();

private:
  bool _on;

  bool _brightnessTuning;
  int _tunedBrightness;

  bool _blink;
  int _blinkInterval;
  int _blinkSharpness;
  float _blinkPhase;
  unsigned long _blinkTime;
};
