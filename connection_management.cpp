#include <ESP8266WiFi.h>

#include "connection_management.h"
#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "led_handler.h"
#include "server_essentials.h"
#include "wifi_credentials.h"

void initialiseConnectionManagement() {
  initialiseEeprom();

  // Setup the WiFi module's LED settings
  initialiseLedHandler();
  setLedHandlerState(STATE_CONNECTING);
  
  // TODO: Uncomment the below to read network details from EEPROM, and remove the header to wifi_credentials.
  // Read Network SSID and Passcode from EEPROM
  // char ssid[MAX_SSID_LENGTH];
  // char password[MAX_KEY_LENGTH];
  // getNetworkSsid(ssid);
  // getNetworkKey(password);

  if (ssid[0] == '\0') {
    setLedHandlerState(STATE_FAILED);
    while(1) {
      delay(1000);
    }
  }

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (WiFi.status() == WL_CONNECT_FAILED) {
      setLedHandlerState(STATE_FAILED);
      setBoardLedState(false);
      while(1){
        delay(2000);
      }
    }
  }
  setLedHandlerState(STATE_CONNECTED);

  // Start the server
  server.begin();
}

void saveNetworkSsid(const char* newSsid) {
  writeTextToEeprom(getSsidMemoryLocation(), newSsid, SSID_MAX_LENGTH);
}

void getNetworkSsid(char* dst) {
  getTextFromEeprom(getSsidMemoryLocation(), dst, SSID_MAX_LENGTH);
}

void saveNetworkKey(const char* newKey) {
  writeTextToEeprom(getNetworkKeyMemoryLocation(), newKey, KEY_MAX_LENGTH);
}

void getNetworkKey(char* dst) {
  getTextFromEeprom(getNetworkKeyMemoryLocation(), dst, KEY_MAX_LENGTH);
}