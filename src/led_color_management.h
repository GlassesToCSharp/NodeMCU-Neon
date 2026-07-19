#ifndef LED_COLOR_MANAGEMENT_H
#define LED_COLOR_MANAGEMENT_H

#include <Arduino.h>

static const char* jsonKeyColor = "led-color";

extern void initialiseLedColorManagement();
extern void registerLedColorManagement();
extern uint32_t getLedColor();
extern void fadeColor();

#endif