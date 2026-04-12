#include <EEPROM.h>

#include "eeprom_handler.h"

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
  EEPROM.commit();
}

uint8_t getByteFromEeprom(const uint16_t address) {
  return (uint8_t)EEPROM.read(address);
}

void writeShortToEeprom(const uint16_t address, uint16_t number) {
  uint8_t upper = number >> 8;
  uint8_t lower = number & 0xFF;
  EEPROM.put(address, upper);
  EEPROM.put(address + 1, lower);
  EEPROM.commit();
}

uint16_t getShortFromEeprom(const uint16_t address) {
  uint16_t upper = (uint16_t)EEPROM.read(address);
  uint8_t lower = (uint8_t)EEPROM.read(address + 1);
  return (upper << 8) | lower;
}

void writeIntToEeprom(const uint16_t address, uint32_t number) {
  uint8_t _24_31 = number >> 24;
  uint8_t _16_23 = number >> 16;
  uint8_t _8_15 = number >> 8;
  uint8_t _0_7 = number & 0xFF;
  EEPROM.put(address, _24_31);
  EEPROM.put(address + 1, _16_23);
  EEPROM.put(address + 2, _8_15);
  EEPROM.put(address + 3, _0_7);
  EEPROM.commit();
}

uint32_t getIntFromEeprom(const uint16_t address) {
  uint32_t _24_31 = (uint32_t)EEPROM.read(address);
  uint32_t _16_23 = (uint32_t)EEPROM.read(address + 1);
  uint32_t _8_15 = (uint32_t)EEPROM.read(address + 2);
  uint32_t _0_7 = (uint32_t)EEPROM.read(address + 3);
  return (_24_31 << 24) |
        (_16_23 << 16) |
        (_8_15 << 8) |
        _0_7;
}
