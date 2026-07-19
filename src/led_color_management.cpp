#include <Arduino.h>
#include <ArduinoJson.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "feature_management.h"
#include "led_color_management.h"
#include "pwm_management.h"
#include "server_essentials.h"

const char* const jsonKeyColor = "led-color";

static const Feature _feature = LED_STRIP;
static const int redPin = D1;
static const int greenPin = D2;
static const int bluePin = D3;

static volatile uint32_t _currentColor = 0x00000000;
uint8_t fadeArrayRed[fadeStepCount];
uint8_t fadeArrayGreen[fadeStepCount];
uint8_t fadeArrayBlue[fadeStepCount];
static volatile bool requiresFade = false;
static volatile uint32_t timeSinceLastFadeUpdate = 0;
static volatile uint16_t fadeStepTracker = 0;

void _handleLedColor();
uint32_t _to32BitColor(const uint8_t * red, const uint8_t * green, const uint8_t * blue);
void _createFadeTransition(const uint32_t * newColor);

void initialiseLedColorManagement() {
  setupPwm(redPin);
  setupPwm(greenPin);
  setupPwm(bluePin);
  _currentColor = 0x00000000;
}

void registerLedColorManagement() {
  server.on("/led-color", HTTP_POST, _handleLedColor);
}

uint32_t getLedColor() {
  return _currentColor;
}

void fadeColor() {
  if (!requiresFade) {
    return;
  }

  // Add a delay to show the fade. Otherwise, it will fade too quickly.
  // `analogWrite` takes ~2ms to run. 3 calls take ~6ms.
  if ((millis() - timeSinceLastFadeUpdate) < (fadeTransitionDelay - 6U)){
    return;
  }

  analogWrite(redPin, *(fadeArrayRed + fadeStepTracker));
  analogWrite(greenPin, *(fadeArrayGreen + fadeStepTracker));
  analogWrite(bluePin, *(fadeArrayBlue + fadeStepTracker));
  
  timeSinceLastFadeUpdate = millis();
  _currentColor = _to32BitColor(
    (fadeArrayRed + fadeStepTracker), 
    (fadeArrayGreen + fadeStepTracker), 
    (fadeArrayBlue + fadeStepTracker));
  
  fadeStepTracker++;
  
  if (fadeStepTracker >= fadeStepCount) {
    // Fade complete. Prevet any further updates to the fade.
    requiresFade = false;
  }
}

static void _onColorSuccess(JsonDocument* doc) {
  uint32_t newColor = (*doc)[jsonKeyColor].as<uint32_t>();
  _createFadeTransition(&newColor);
  fadeStepTracker = 0;
  requiresFade = true;
}

void _handleLedColor() {
  handleHttpPostWithFeatureEnablement(jsonKeyColor, _feature, _onColorSuccess);
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

void _createFadeTransition(const uint32_t * newColor) {
  // Now that we have the colour, we will need to split it between red, green,
  // blue, and opacity. Once that is done, the LEDs need to be set to the right
  // colour. Format of the 32-bit INT is xRGB (where x is ignored). Example:
  // colour RED is 0x00FF0000.

  // Take away the volatility here.
  uint32_t currentColor = _currentColor;
  uint8_t currentRed = _toRed(&currentColor);
  uint8_t currentGreen = _toGreen(&currentColor);
  uint8_t currentBlue = _toBlue(&currentColor);

  uint8_t newRed = _toRed(newColor);
  uint8_t newGreen = _toGreen(newColor);
  uint8_t newBlue = _toBlue(newColor);

  interpolateFade(&currentRed, &newRed, fadeArrayRed);
  interpolateFade(&currentGreen, &newGreen, fadeArrayGreen);
  interpolateFade(&currentBlue, &newBlue, fadeArrayBlue);
}