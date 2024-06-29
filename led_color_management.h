#ifndef LED_COLOR_MANAGEMENT_H
#define LED_COLOR_MANAGEMENT_H

#include <Arduino.h>

const uint8_t LED_COLOR_MAX_LENGTH = 4;

extern void initialiseLedColorManagement();
extern void registerLedColorManagement();
extern uint16_t getLedColor();

#endif