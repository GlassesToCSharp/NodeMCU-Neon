#include <Arduino.h>
#include <ArduinoJson.h>

#include "pinouts.h"
#include "power_management.h"
#include "server_essentials.h"

static const int powerPin = D5;

// default value of "off"
static bool powerStatus = false;
static const char* jsonKey = "state";
static const Feature _feature = GENERAL_POWER;

void _handlePowerStatus();

void initialisePowerManagement() {
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, powerStatus);
}

void registerPowerManagement() {
  server.on("/power", HTTP_POST, _handlePowerStatus);
}

bool getPowerStatus() {
  return powerStatus;
}

static void _onSuccess(JsonDocument* doc) {
  powerStatus = (*doc)[jsonKey].as<bool>();
  // Set power state of components
  digitalWrite(powerPin, powerStatus);
}

void _handlePowerStatus() {
  handleHttpPostWithFeatureEnablement(jsonKey, _feature, _onSuccess);
}