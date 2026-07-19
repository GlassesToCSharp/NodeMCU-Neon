#ifndef LED_COLOR_MANAGEMENT_H
#define LED_COLOR_MANAGEMENT_H

#include <Arduino.h>

extern const char* const jsonKeyColor;

extern void initialiseLedColorManagement();
extern void registerLedColorManagement();
extern uint32_t getLedColor();
extern void fadeColor();

#endif