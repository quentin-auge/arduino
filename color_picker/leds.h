#pragma once

#include <Arduino.h>

int forceLed(int ledPin, int currentBrightness);
void blipLed(int ledPin, int currentBrightness);

void setRGBFromPitch(int pitch);