#ifndef SERVER_ESSENTIALS_H
#define SERVER_ESSENTIALS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

extern ESP8266WebServer server;

extern const char* contentTypePlain;
extern const char* contentTypeJson;
extern const char* emptyResponse;
extern const uint16_t failStatusCode;
extern const uint16_t successStatusCode;
extern const uint16_t successEmptyStatusCode;

extern void handleHttpPost(const char* key, void (*onSuccess)(JsonDocument*));

#endif