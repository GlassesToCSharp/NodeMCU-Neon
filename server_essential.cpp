#include "server_essentials.h"

ESP8266WebServer server(80);

const char* contentTypePlain = "text/plain";
const char* contentTypeJson = "application/json";
const char* emptyResponse = "";
const uint16_t failStatusCode = 400;
const uint16_t successStatusCode = 200;
const uint16_t successEmptyStatusCode = 204;