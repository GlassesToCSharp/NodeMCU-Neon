#include <EEPROM.h>

#include "eeprom_handler.h"

enum MemoryLocations {
  SSID = 0,
  KEY = SSID_MAX_LENGTH,
};

bool setupComplete = false;

void initialiseEeprom() {
  if (setupComplete) {
    return;
  }

  EEPROM.begin(512);
  setupComplete = true;
}

void writeTextToEeprom(const uint16_t address, const char* text, const uint8_t maxLength) {
  for(int i = 0; i < maxLength; i++) {
    if (i >= maxLength-1 || text[i] == '\0') {
      // End the string with the null character
      EEPROM.put(address + i, '\0');
      break;
    }

    EEPROM.write(address + i, text[i]);
  }

  EEPROM.commit();
}

void getTextFromEeprom(const uint16_t address, char* text, const uint8_t maxLength) {
  for(int i = 0; i < maxLength; i++) {
    char character = EEPROM.read(address + i);
    if (i >= maxLength-1 || character == '\0') {
      // End the string with the null character
      text[i] = '\0';
      break;
    }

    text[i] = character;
  }
}

void saveNetworkSsid(const char* ssid) {
  writeTextToEeprom((int)SSID, ssid, SSID_MAX_LENGTH);
}

void saveNetworkKey(const char* key) {
  writeTextToEeprom((int)KEY, key, KEY_MAX_LENGTH);
}

void getNetworkSsid(char* ssid) {
  getTextFromEeprom((int)SSID, ssid, SSID_MAX_LENGTH);
}

void getNetworkKey(char* key) {
  getTextFromEeprom((int)KEY, key, KEY_MAX_LENGTH);
}
