#include <Arduino.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "neon_management.h"
#include "pinouts.h"
#include "pwm_management.h"
#include "server_essentials.h"

const char* const jsonKeyNeon = "neon-brightness";

static const Feature _feature = NEON;
static const int neonPin = D0;

static volatile uint8_t _currentBrightness = 0;
uint8_t brightnessArray[fadeStepCount];
static volatile bool requiresFade = false;
static volatile uint32_t timeSinceLastFadeUpdate = 0;
static volatile uint16_t fadeStepTracker = 0;


void _handleNeonBrightness();
void _createFadeTransition(const uint8_t * newBrightness);

void initialiseNeonManagement() {
  setupPwm(neonPin);
}

void registerNeonManagement() {
  server.on("/neon-brightness", HTTP_POST, _handleNeonBrightness);
}

uint8_t getNeonBrightness() {
  return _currentBrightness;
}

void fadeNeon() {
  if (!requiresFade) {
    return;
  }

  // Add a delay to show the fade. Otherwise, it will fade too quickly.
  // `analogWrite` takes ~2ms to run.
  if ((millis() - timeSinceLastFadeUpdate) < (fadeTransitionDelay - 2U)){
    return;
  }
  
  analogWrite(neonPin, *(brightnessArray + fadeStepTracker));
  
  timeSinceLastFadeUpdate = millis();
  _currentBrightness = *(brightnessArray + fadeStepTracker);
  
  fadeStepTracker++;
  
  if (fadeStepTracker >= fadeStepCount) {
    // Fade complete. Prevet any further updates to the fade.
    requiresFade = false;
  }
}

static void _onSuccess(JsonDocument* doc) {
  uint8_t brightness = (*doc)[jsonKeyNeon].as<uint8_t>();
  _createFadeTransition(&brightness);
  fadeStepTracker = 0;
  requiresFade = true;
}

void _handleNeonBrightness() {
  handleHttpPostWithFeatureEnablement(jsonKeyNeon, _feature, _onSuccess);
}


void _createFadeTransition(const uint8_t * newBrightness) {
  uint8_t currentBrightness = _currentBrightness;
  interpolateFade(&currentBrightness, newBrightness, brightnessArray);
}
