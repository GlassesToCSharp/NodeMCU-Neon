#include <ArduinoJson.h>

#include "device_management.h"

// Private variables
char _deviceName[20] = "Default Name";

// Private functions
void _handleNewName();
void _setName(const char* deviceName);

// Function Definitions

void initialiseDeviceManagement() {
  initialiseEeprom();
  server.on("/name", HTTP_POST, _handleNewName);
}

void getDeviceName(char* dst) {
  const uint16_t address = getDeviceNameMemoryLocation();
  getTextFromEeprom(address, dst, DEVICE_NAME_MAX_LENGTH);
}

void _setName(const char* deviceName) {
  const uint16_t address = getDeviceNameMemoryLocation();
  writeTextToEeprom(address, deviceName, DEVICE_NAME_MAX_LENGTH);
}

void _handleNewName() {
  uint16_t statusCode = failStatusCode;
  if (server.hasArg("plain")) {
    String json = server.arg("plain");
    JsonDocument doc;
    deserializeJson(doc, json);
    char deviceName[DEVICE_NAME_MAX_LENGTH];
    strncpy(deviceName, doc["name"], sizeof(deviceName));
    statusCode = successEmptyStatusCode;
    // Save the new name to EEPROM
    _setName(deviceName);
  }
  server.send(statusCode, contentTypePlain, emptyResponse); 
}