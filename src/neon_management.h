#ifndef NEON_MANAGEMENT_H
#define NEON_MANAGEMENT_H

#include <Arduino.h>

// As the brightness is governed by PWM and the range is 0<=PWM<256, this
// should not be more than one byte.
const uint8_t NEON_BRIGHTNESS_MAX_LENGTH = 1;

extern void initialiseNeonManagement();
extern void registerNeonManagement();
extern uint8_t getNeonBrightness();

#endif