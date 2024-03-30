#include <Arduino.h>
#include <EEPROM.h>

#include "eeprom_handler.h"

enum MemoryLocations {
  SSID = 0,
  KEY = MAX_SSID_LENGTH,
};

void initialiseEeprom() {
  EEPROM.begin(512);
}

void _writeTextToEeprom(const int address, const char* text, const int maxLength) {
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

void _getTextFromEeprom(const int address, char* text, const int maxLength) {
  Serial.print("Address: "); Serial.println(address);
  Serial.print("Max Length: "); Serial.println(maxLength);
  for(int i = 0; i < maxLength; i++) {
    char character = EEPROM.read(address + i);
    Serial.print(character);
    if (i >= maxLength-1 || character == '\0') {
      // End the string with the null character
      text[i] = '\0';
      break;
    }

    text[i] = character;
  }
  Serial.println();
}

void saveNetworkSsid(const char* ssid) {
  _writeTextToEeprom((int)SSID, ssid, MAX_SSID_LENGTH);
}

void saveNetworkKey(const char* key) {
  _writeTextToEeprom((int)KEY, key, MAX_KEY_LENGTH);
}

void getNetworkSsid(char* ssid) {
  _getTextFromEeprom((int)SSID, ssid, MAX_SSID_LENGTH);
}

void getNetworkKey(char* key) {
  _getTextFromEeprom((int)KEY, key, MAX_KEY_LENGTH);
}
