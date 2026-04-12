#ifndef EEPROM_MEMORY_MANAGEMENT_H
#define EEPROM_MEMORY_MANAGEMENT_H

#include <Arduino.h>

extern uint16_t getSsidMemoryLocation();
extern uint16_t getNetworkKeyMemoryLocation();
extern uint16_t getDeviceNameMemoryLocation();
extern uint16_t getFeaturesMemoryLocation();
extern uint16_t getMotorSpeedMemoryLocation();
extern uint16_t getMotorAccelerationMemoryLocation();

#endif