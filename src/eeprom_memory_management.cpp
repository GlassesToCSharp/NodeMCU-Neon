#include <Arduino.h>

#include "connection_management.h"
#include "device_management.h"
#include "eeprom_handler.h"
#include "led_color_management.h"
#include "motor_management.h"
#include "neon_management.h"

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

// Fourth item in the memory location
uint16_t getFeaturesMemoryLocation() {
  return getDeviceNameMemoryLocation() + DEVICE_NAME_MAX_LENGTH;
}

// Fifth item in the memory location
uint16_t getMotorSpeedMemoryLocation() {
  return getFeaturesMemoryLocation() + NEON_BRIGHTNESS_MAX_LENGTH;
}

// Sixth item in the memory location
uint16_t getMotorAccelerationMemoryLocation() {
  return getMotorSpeedMemoryLocation() + MOTOR_SPEED_MAX_LENGTH;
}