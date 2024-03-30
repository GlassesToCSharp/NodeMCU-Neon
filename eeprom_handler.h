#ifndef EEPROM_HANDLER_H
#define EEPROM_HANDLER_H

const int MAX_SSID_LENGTH = 20;
const int MAX_KEY_LENGTH = 20;

void initialiseEeprom(void);
void saveNetworkSsid(const char* ssid);
void saveNetworkKey(const char* key);
void getNetworkSsid(char* ssid);
void getNetworkKey(char* key);

#endif