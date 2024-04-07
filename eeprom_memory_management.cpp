#include "eeprom_memory_management.h"

// First item in the memory location
uint16_t getSsidMemoryLocation() {
  return 0;
}

// Second item in the memory location
uint16_t getNetworkKeyMemoryLocation() {
  return getSsidMemoryLocation() + SSID_MAX_LENGTH;
}

// Third item in the memory location
uint16_t getDeviceNameMemoryLocation() {
  return getNetworkKeyMemoryLocation() + KEY_MAX_LENGTH;
}