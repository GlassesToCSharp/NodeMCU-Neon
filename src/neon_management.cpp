#include <Arduino.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "pinouts.h"
#include "pwm_management.h"
#include "server_essentials.h"

static const int neonPin = D3;
static uint8_t currentBrightness = 0;
uint8_t brightnessArray[fadeStepCount];

static const char* jsonKey = "neon-brightness";

void _handleNeonBrightness();
void _fadeTo(const uint8_t * newBrightness);

void initialiseNeonManagement() {
  setupPwm(neonPin);
}

void registerNeonManagement() {
  server.on("/neon-brightness", HTTP_POST, _handleNeonBrightness);
}

uint8_t getNeonBrightness() {
  return currentBrightness;
}

static void _onSuccess(JsonDocument* doc) {
  uint8_t brightness = (*doc)[jsonKey].as<uint8_t>();
  // Set PWM of Neon brightness
  _fadeTo(&brightness);
}

void _handleNeonBrightness() {
  handleHttpPost(jsonKey, _onSuccess);
}

void _fadeTo(const uint8_t * newBrightness) {
  interpolateFade(&currentBrightness, newBrightness, brightnessArray);

  for(int i = 0; i < fadeStepCount; i++) {
    analogWrite(neonPin, *(brightnessArray + i));
    // Add a delay to show the fade. Otherwise, it will fade too quickly.
    // `analogWrite` takes ~2ms to run.
    delay(fadeTransitionDelay - 2);
    currentBrightness = *(brightnessArray + i);
  }
}