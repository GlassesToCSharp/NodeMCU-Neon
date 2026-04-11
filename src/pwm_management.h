#ifndef PWM_MANAGEMENT_H
#define PWM_MANAGEMENT_H

#include <Arduino.h>

extern const uint16_t fadeStepCount;
extern const uint16_t fadeTransitionDelay;

extern void setupPwm(uint8_t pin);
extern void interpolateFade(const uint8_t * firstValue, const uint8_t * secondValue, uint8_t * dst);

#endif