#include <Arduino.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "pinouts.h"
#include "pwm_management.h"
#include "server_essentials.h"

static const int neonPin = D3;

static const char* jsonKey = "neon-brightness";

void _handleNeonBrightness();

void initialiseNeonManagement() {
  setupPwm(neonPin);
  initialiseEeprom();
}

void registerNeonManagement() {
  server.on("/neon-brightness", HTTP_POST, _handleNeonBrightness);
}

uint8_t getNeonBrightness() {
    return getByteFromEeprom(getNeonBrightnessMemoryLocation());
}

static void _onSuccess(JsonDocument* doc) {
  uint8_t brightness = (*doc)[jsonKey].as<uint8_t>();
  writeByteToEeprom(getNeonBrightnessMemoryLocation(), brightness);
  // Set PWM of Neon brightness
  // TODO: Add fade?
  analogWrite(neonPin, brightness);
}

void _handleNeonBrightness() {
  handleHttpPost(jsonKey, _onSuccess);
}