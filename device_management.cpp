#include <ArduinoJson.h>

#include "device_management.h"
#include "server_essentials.h"

// Private variables
char _deviceName[DEVICE_NAME_MAX_LENGTH] = "Default Name";
static const char* _jsonKey = "name";

// Private functions
void _handleNewName();

// Function Definitions

void initialiseDeviceManagement() {
  initialiseEeprom();
  server.on("/name", HTTP_POST, _handleNewName);
}

void getDeviceName(char* dst) {
  const uint16_t address = getDeviceNameMemoryLocation();
  getTextFromEeprom(address, dst, DEVICE_NAME_MAX_LENGTH);
}

static void _onSuccess(JsonDocument* doc) {
  strncpy(_deviceName, (*doc)["name"], sizeof(_deviceName));
  writeTextToEeprom(getDeviceNameMemoryLocation(), _deviceName, DEVICE_NAME_MAX_LENGTH);
}

void _handleNewName() {
  handleHttpPost(_jsonKey, _onSuccess);
}