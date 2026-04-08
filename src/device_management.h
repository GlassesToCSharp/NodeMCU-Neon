#ifndef DEVICE_MANAGEMENT_H
#define DEVICE_MANAGEMENT_H

#include <Arduino.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "server_essentials.h"

const uint8_t DEVICE_NAME_MAX_LENGTH = 20;

extern void initialiseDeviceManagement();
extern void getDeviceName(char* dst);

#endif