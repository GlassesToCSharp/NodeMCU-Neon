#ifndef EEPROM_MEMORY_MANAGEMENT_H
#define EEPROM_MEMORY_MANAGEMENT_H

#include <Arduino.h>

#include "device_management.h"
#include "eeprom_handler.h"

extern uint16_t getSsidMemoryLocation();
extern uint16_t getNetworkKeyMemoryLocation();
extern uint16_t getDeviceNameMemoryLocation();

#endif