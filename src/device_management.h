#ifndef DEVICE_MANAGEMENT_H
#define DEVICE_MANAGEMENT_H

#include <Arduino.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "server_essentials.h"

const uint8_t DEVICE_NAME_MAX_LENGTH = 20;
// 3 chars + '\0'
// [0-9][a-z][A-Z] = 62 possibilities per char
// 62*62*62 = 62^3 = 238,328 possible values
// That should be plenty.
const uint8_t DEVICE_ID_MAX_LENGTH = 4;

static const char* jsonKeyName = "name";
static const char* jsonKeyId = "id";

extern void initialiseDeviceManagement();
extern void getDeviceName(char* dst);
extern void getDeviceId(char* dst);

#endif