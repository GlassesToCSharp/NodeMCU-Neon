#include <Arduino.h>
#include <ArduinoJson.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "pwm_management.h"
#include "server_essentials.h"

static const char* _colorJsonKey = "color";

static uint32_t currentColor = 0x00000000;
static const int redPin = D2;
static const int greenPin = D1;
static const int bluePin = D0;
uint8_t fadeArrayRed[fadeStepCount];
uint8_t fadeArrayGreen[fadeStepCount];
uint8_t fadeArrayBlue[fadeStepCount];

void _handleLedColor();
void _fadeTo(const uint32_t * newColor);

void initialiseLedColorManagement() {
  setupPwm(redPin);
  setupPwm(greenPin);
  setupPwm(bluePin);
  currentColor = 0x00000000;
}

void registerLedColorManagement() {
  server.on("/led-color", HTTP_POST, _handleLedColor);
}

uint32_t getLedColor() {
  return currentColor;
}

static void _onColorSuccess(JsonDocument* doc) {
  uint32_t newColor = (*doc)[_colorJsonKey].as<uint32_t>();
  // Now that we have the colour, we will need to split it between red, green,
  // blue, and opacity. Once that is done, the LEDs need to be set to the right
  // colour. Format of the 32-bit INT is xRGB (where x is ignored). Example:
  // colour RED is 0x00FF0000.
  _fadeTo(&newColor);
}

void _handleLedColor() {
  handleHttpPost(_colorJsonKey, _onColorSuccess);
}

uint8_t _toRed(const uint32_t * color) {
  return (*color >> 16) & 0xFF;
}

uint8_t _toGreen(const uint32_t * color) {
  return (*color >> 8) & 0xFF;
}

uint8_t _toBlue(const uint32_t * color) {
  return *color & 0xFF;
}

uint32_t _to32BitColor(const uint8_t * red, const uint8_t * green, const uint8_t * blue) {
  return 0x00000000 | 
    (*red << 16) |
    (*green << 8) |
    (*blue);
}

void _fadeTo(const uint32_t * newColor) {
  uint8_t currentRed = _toRed(&currentColor);
  uint8_t currentGreen = _toGreen(&currentColor);
  uint8_t currentBlue = _toBlue(&currentColor);

  uint8_t newRed = _toRed(newColor);
  uint8_t newGreen = _toGreen(newColor);
  uint8_t newBlue = _toBlue(newColor);

  interpolateFade(&currentRed, &newRed, fadeArrayRed);
  interpolateFade(&currentGreen, &newGreen, fadeArrayGreen);
  interpolateFade(&currentBlue, &newBlue, fadeArrayBlue);

  for(int i = 0; i < fadeStepCount; i++) {
    analogWrite(redPin, *(fadeArrayRed + i));
    analogWrite(greenPin, *(fadeArrayGreen + i));
    analogWrite(bluePin, *(fadeArrayBlue + i));
    // Add a delay to show the fade. Otherwise, it will fade too quickly.
    // `analogWrite` takes ~2ms to run. 3 calls take ~6ms. 
    delay(fadeTransitionDelay - 6);
    currentColor = _to32BitColor((fadeArrayRed + i), (fadeArrayGreen + i), (fadeArrayBlue + i));
  }
}