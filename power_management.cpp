#include <ArduinoJson.h>

#include "power_management.h"
#include "server_essentials.h"

bool powerStatus = false;
const char* jsonKey = "state";

void _handlePowerStatus();

void initialisePowerManagement() {
  server.on("/power", HTTP_POST, _handlePowerStatus);
}

void _onSuccess(JsonDocument* doc) {
  powerStatus = (*doc)[jsonKey].as<bool>();
  // TODO: Set power state of components
  // Use digitalWrite(PowerPin, value) ?
}

void _handlePowerStatus() {
  handleHttpPost(jsonKey, _onSuccess);
}