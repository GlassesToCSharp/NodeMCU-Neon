#include <Arduino.h>

#include "pwm_management.h"

const uint16_t fadeDurationMs = 2000;
// const uint16_t fadeStepCount = 1024;
const uint16_t fadeTransitionDelay = fadeDurationMs / fadeStepCount;

void setupPwm(uint8_t pin) {
  pinMode(pin, OUTPUT);
  analogWriteRange(255); 
  analogWriteFreq(500); // reduce to 500 Hz for better compatibility with hardware
  analogWrite(pin, 0); // Default to completely off
}

void interpolateFade(const uint8_t * firstValue, const uint8_t * secondValue, uint8_t * dst) {
  double step = 1.0 / (fadeStepCount - 1);
  double fadeStep = (double)((*secondValue) - (*firstValue)) * step;
  // first value in the destination is the first value in the source.
  *dst = *firstValue;
  for (uint16_t i = 1; i < fadeStepCount; i++) {
    dst++;
    uint8_t addedDifference = fadeStep * (double)i;
    *dst = (*firstValue) + addedDifference;
  }
}