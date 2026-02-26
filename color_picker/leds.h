#pragma once

#include <Arduino.h>

#define MIN_BRIGHTNESS 1
#define MAX_BRIGHTNESS 255

int forceLed(int ledPin, int currentBrightness);
void blipLed(int ledPin, int currentBrightness);

void setRGBFromPitch(int pitch);