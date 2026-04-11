#include <Arduino.h>
#include <ArduinoJson.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "pwm_management.h"
#include "server_essentials.h"

static const char* _colorJsonKey = "color";

static uint32_t color = 0x00000000;
static const int redPin = D2;
static const int greenPin = D1;
static const int bluePin = D0;

void _handleLedColor();

void initialiseLedColorManagement() {
  setupPwm(redPin);
  setupPwm(greenPin);
  setupPwm(bluePin);
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
  // Now that we have the colour, we will need to split it between red, green,
  // blue, and opacity. Once that is done, the LEDs need to be set to the right
  // colour. Format of the 32-bit INT is xRGB (where x is ignored). Example:
  // colour RED is 0x00FF0000.
  uint8_t red = (color >> 16) & 0xFF;
  uint8_t green = (color >> 8) & 0xFF;
  uint8_t blue = color & 0xFF;

  // TODO: Fade?
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void _handleLedColor() {
  handleHttpPost(_colorJsonKey, _onColorSuccess);
}