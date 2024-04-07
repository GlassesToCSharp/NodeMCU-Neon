#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include "connection_management.h"
#include "device_management.h"
#include "motor_management.h"
#include "neon_management.h"
#include "power_management.h"
#include "server_essentials.h"
 
void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  initialiseConnectionManagement();
  // TODO: Initialise power state (default off)
  // TODO: Initialise motors
 
  // Print the IP address
  Serial.print(F("Use this URL to connect: http://"));
  Serial.print(WiFi.localIP());
  Serial.println(F("/"));
  
  // Handlers
  initialiseDeviceManagement();
  initialisePowerManagement();
  initialiseNeonManagement();
  initialiseMotorManagement();
  initialiseSummary();
}
 
void loop() {
  // Check if a client has connected
  server.handleClient();
}

//////////////////////

void initialiseSummary() {
  server.on("/status", HTTP_GET, handleStatus);
}

void handleStatus() {
  // Size is determined using the ArduinJson Assistant:
  // https://arduinojson.org/v6/assistant
  // JSON structure:
  // {
  //   "name": "Device Name        ", // Max size: 19 chars long (20 including '\0' char)
  //   "power": false,
  //   "neon": 255,
  //   "motor": {
  //     "speed": 1234,
  //     "position": 1234,
  //     "acceleration": 1234
  //   }
  // }
  StaticJsonDocument<192> doc;

  char deviceName[20];
  getDeviceName(deviceName);
  doc["name"] = deviceName;
  doc["power"] = getPowerStatus();
  doc["neon"] = getNeonBrightness();

  JsonObject motor = doc.createNestedObject("motor");
  motor["speed"] = getMotorSpeed();
  motor["position"] = getMotorPosition();
  motor["acceleration"] = getMotorAcceleration();

  char output[128];
  serializeJson(doc, output);
  server.send(successStatusCode, contentTypeJson, output); 
}
