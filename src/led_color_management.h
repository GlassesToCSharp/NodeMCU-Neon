#ifndef LED_COLOR_MANAGEMENT_H
#define LED_COLOR_MANAGEMENT_H

#include <Arduino.h>

extern void initialiseLedColorManagement();
extern void registerLedColorManagement();
extern uint16_t getLedColor();

#endif