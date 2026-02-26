#pragma once

#include <Arduino.h>

#define BLINK_INTERVAL 500

int forceLed(int ledPin, int currentBrightness);
void blipLed(int ledPin, int currentBrightness);

void setRGBFromPitch(int pitch);