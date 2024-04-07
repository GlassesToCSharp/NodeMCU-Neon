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

void writeByteToEeprom(const uint16_t address, uint8_t number) {
  EEPROM.put(address, number);
}

uint8_t getByteFromEeprom(const uint16_t address) {
  return (uint8_t)EEPROM.read(address);
}

void writeIntToEeprom(const uint16_t address, uint16_t number) {
  uint8_t upper = number >> 8;
  uint8_t lower = number & 255;
  EEPROM.put(address, upper);
  EEPROM.put(address + 1, lower);
}

uint16_t getIntFromEeprom(const uint16_t address) {
  uint16_t upper = (uint16_t)EEPROM.read(address);
  uint8_t lower = (uint8_t)EEPROM.read(address + 1);
  return (upper << 8) | lower;
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
