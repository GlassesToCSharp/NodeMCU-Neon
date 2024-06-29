#include <Arduino.h>
#include <ArduinoJson.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "server_essentials.h"

static const char* _colorJsonKey = "color";

static uint32_t color = 0x00000000;

void _handleLedColor();

void initialiseLedColorManagement() {
  // TODO: Setup LED colour handling, depending on whether the LED strip is PWM
  // or addressable LEDs
  color = getIntFromEeprom(getLedColorMemoryLocation());
}

void registerLedColorManagement() {
  server.on("/led-color", HTTP_POST, _handleLedColor);
}

uint32_t getLedColor() {
  return getIntFromEeprom(getLedColorMemoryLocation());
}

static void _onColorSuccess(JsonDocument* doc) {
  color = (*doc)[_colorJsonKey].as<uint32_t>();
  writeIntToEeprom(getLedColorMemoryLocation(), color);
  // TODO: Set the LED color.
  // Now that we have the colour, we will need to split it between red, green,
  // blue, and opacity. Once that is done, the LEDs need to be set to the right
  // colour, but this will depend on the LED structure (PWM or signal).
}

void _handleLedColor() {
  handleHttpPost(_colorJsonKey, _onColorSuccess);
}