#ifndef CONNECTION_MANAGEMENT_H
#define CONNECTION_MANAGEMENT_H

#include <Arduino.h>

const uint8_t SSID_MAX_LENGTH = 20;
const uint8_t KEY_MAX_LENGTH = 20;

extern void initialiseConnectionManagement();
extern void checkForUdpBroadcastMessage();

extern void saveNetworkSsid(const char* newSsid);
extern void getNetworkSsid(char* dst);

extern void saveNetworkKey(const char* newKey);
extern void getNetworkKey(char* dst);

#endif