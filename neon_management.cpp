#include <Arduino.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "server_essentials.h"

static const char* jsonKey = "neon-brightness";

void _handleNeonBrightness();

void initialiseNeonManagement() {
  initialiseEeprom();
  server.on("/neon-brightness", HTTP_POST, _handleNeonBrightness);
}

uint8_t getNeonBrightness() {
    return getByteFromEeprom(getNeonBrightnessMemoryLocation());
}

static void _onSuccess(JsonDocument* doc) {
  uint8_t brightness = (*doc)[jsonKey].as<uint8_t>();
  writeByteToEeprom(getNeonBrightnessMemoryLocation(), brightness);
  // TODO: Set PWM of Neon brightness
  // Use analogWrite(NeonPin, value) ?
}

void _handleNeonBrightness() {
  handleHttpPost(jsonKey, _onSuccess);
}