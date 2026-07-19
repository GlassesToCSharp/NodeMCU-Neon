#include <ArduinoJson.h>

#include "device_management.h"
#include "server_essentials.h"

const char* const jsonKeyName = "name";
const char* const jsonKeyId = "id";

// Private variables
char _deviceName[DEVICE_NAME_MAX_LENGTH] = "Default Name";
char _deviceId[DEVICE_ID_MAX_LENGTH] = "";

// Private functions
void _handleNewName();
void _setDeviceName(char* src);
void _handleNewId();
void _setDeviceId(char* src);

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
  // Same with the device ID.
  char tempDeviceId[DEVICE_ID_MAX_LENGTH];
  getDeviceId(tempDeviceId);
  if (tempDeviceId[0] == 0xFF) {
    _setDeviceId(_deviceId);
  }
  server.on("/name", HTTP_POST, _handleNewName);
  server.on("/id", HTTP_POST, _handleNewId);
}

void getDeviceName(char* dst) {
  const uint16_t address = getDeviceNameMemoryLocation();
  getTextFromEeprom(address, dst, DEVICE_NAME_MAX_LENGTH);
}

void getDeviceId(char* dst) {
  const uint16_t address = getDeviceIdMemoryLocation();
  getTextFromEeprom(address, dst, DEVICE_ID_MAX_LENGTH);
}

void _setDeviceName(char* src) {
  const uint16_t address = getDeviceNameMemoryLocation();
  writeTextToEeprom(address, src, DEVICE_NAME_MAX_LENGTH);
}

void _setDeviceId(char* src) {
  const uint16_t address = getDeviceIdMemoryLocation();
  writeTextToEeprom(address, src, DEVICE_ID_MAX_LENGTH);
}

static void _onNameSuccess(JsonDocument* doc) {
  strncpy(_deviceName, (*doc)[jsonKeyName], sizeof(_deviceName));
  _setDeviceName(_deviceName);
}

static void _onIdSuccess(JsonDocument* doc) {
  strncpy(_deviceId, (*doc)[jsonKeyId], sizeof(_deviceId));
  _setDeviceId(_deviceId);
}

void _handleNewName() {
  handleHttpPost(jsonKeyName, _onNameSuccess);
}

void _handleNewId() {
  handleHttpPost(jsonKeyId, _onIdSuccess);
}