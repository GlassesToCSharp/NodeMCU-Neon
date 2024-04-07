#ifndef EEPROM_HANDLER_H
#define EEPROM_HANDLER_H

#include <Arduino.h>

const uint8_t SSID_MAX_LENGTH = 20;
const uint8_t KEY_MAX_LENGTH = 20;

extern void initialiseEeprom(void);
extern void writeTextToEeprom(const uint16_t address, const char* text, const uint8_t maxLength);
extern void getTextFromEeprom(const uint16_t address, char* text, const uint8_t maxLength);
extern void saveNetworkSsid(const char* ssid);
extern void saveNetworkKey(const char* key);
extern void getNetworkSsid(char* ssid);
extern void getNetworkKey(char* key);

#endif