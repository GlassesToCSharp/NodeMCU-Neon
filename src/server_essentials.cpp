#include <ArduinoJson.h>

#include "feature_management.h"
#include "server_essentials.h"

ESP8266WebServer server(80);

const char* contentTypePlain = "text/plain";
const char* contentTypeJson = "application/json";
const char* emptyResponse = "";
const uint16_t failStatusCode = 400;
const uint16_t permissionDeniedStatusCode = 403;
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

void handleHttpPostWithFeatureEnablement(const char* key, const Feature feature, void (*onSuccess)(JsonDocument*)) {
  int statusCode = failStatusCode;
  if (server.hasArg("plain")) {
    String json = server.arg("plain");
    JsonDocument doc;
    deserializeJson(doc, json);

    // Check whether the feature is being enabled or disabled.
    JsonVariant v = doc[jsonKeyFeatureStatus];
    if (!v.isNull() && v.is<bool>()) {
      bool enabled = v.as<bool>();
      if (enabled) {
        enableFeature(feature);
        // Continue onwards to update values;
      } else {
        // If the feature has been disabled, any other values passed in don't 
        // matter. Save in memory and respond to client.
        disableFeature(feature);
        server.send(successEmptyStatusCode, contentTypePlain, emptyResponse); 
        return;
      }
    }

    // Check if the feature is enabled. If not, ignore values and respond to
    // client.
    if (!isFeatureEnabled(feature)) {
      server.send(permissionDeniedStatusCode, contentTypePlain, emptyResponse);
      return;
    }

    // The feature is enabled. Check whether the right key exists.
    if (!doc[key].isNull()) {
      (*onSuccess)(&doc);
      statusCode = successEmptyStatusCode;
    }
  }

  server.send(statusCode, contentTypePlain, emptyResponse); 
}