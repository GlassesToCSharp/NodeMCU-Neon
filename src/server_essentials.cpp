#include <ArduinoJson.h>

#include "server_essentials.h"

ESP8266WebServer server(80);

const char* contentTypePlain = "text/plain";
const char* contentTypeJson = "application/json";
const char* emptyResponse = "";
const uint16_t failStatusCode = 400;
const uint16_t successStatusCode = 200;
const uint16_t successEmptyStatusCode = 204;

void handleHttpPost(const char* key, void (*onSuccess)(JsonDocument*)) {
  int statusCode = failStatusCode;
  if (server.hasArg("plain")) {
    String json = server.arg("plain");
    JsonDocument doc;
    deserializeJson(doc, json);
    if (!doc[key].isNull()) {
      (*onSuccess)(&doc);
      statusCode = successEmptyStatusCode;
    }
  }
  server.send(statusCode, contentTypePlain, emptyResponse); 
}