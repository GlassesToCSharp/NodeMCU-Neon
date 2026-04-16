#include <ArduinoJson.h>

#include "device_management.h"
#include "server_essentials.h"

// Private variables
char _deviceName[DEVICE_NAME_MAX_LENGTH] = "Default Name";
static const char* _jsonKey = "name";

// Private functions
void _handleNewName();
void _setDeviceName(char* src);

// Function Definitions

void initialiseDeviceManagement() {
  initialiseEeprom();
  // Check if a value is set in the EEPROM. Default values will be 0xFF. If not
  // set (if first value is 0xFF), set the default name.
  char tempDeviceName[DEVICE_NAME_MAX_LENGTH];
  getDeviceName(tempDeviceName);
  if (tempDeviceName[0] == 0xFF) {
    _setDeviceName(_deviceName);
  }
  server.on("/name", HTTP_POST, _handleNewName);
}

void getDeviceName(char* dst) {
  const uint16_t address = getDeviceNameMemoryLocation();
  getTextFromEeprom(address, dst, DEVICE_NAME_MAX_LENGTH);
  Serial.print("Device name: "); Serial.println(dst);
  Serial.print("Device name in hex: ");
  for (uint8_t i = 0; i < DEVICE_NAME_MAX_LENGTH; i++) {
    Serial.print(dst[i], HEX); Serial.print(" ");
  }
  Serial.println();
}

void _setDeviceName(char* src) {
  const uint16_t address = getDeviceNameMemoryLocation();
  writeTextToEeprom(address, src, DEVICE_NAME_MAX_LENGTH);
}

static void _onSuccess(JsonDocument* doc) {
  strncpy(_deviceName, (*doc)["name"], sizeof(_deviceName));
  _setDeviceName(_deviceName);
}

void _handleNewName() {
  handleHttpPost(_jsonKey, _onSuccess);
}