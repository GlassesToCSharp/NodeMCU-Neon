#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include "connection_management.h"
#include "device_management.h"
#include "led_color_management.h"
#include "motor_management.h"
#include "neon_management.h"
#include "power_management.h"
#include "server_essentials.h"

void initialiseSummary();
void handleStatus();

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  initialisePowerManagement();
  initialiseLedColorManagement();
  initialiseNeonManagement();
  initialiseMotorManagement();
  initialiseConnectionManagement();
 
  // Print the IP address
  Serial.print(F("Use this URL to connect: http://"));
  Serial.print(WiFi.localIP());
  Serial.println(F("/"));
  
  // Handlers
  initialiseDeviceManagement();
  registerPowerManagement();
  registerNeonManagement();
  registerMotorManagement();
  registerLedColorManagement();
  initialiseSummary();
}
 
void loop() {
  // Handle the MDNS service
  MDNS.update();
  // Check if a client has connected
  server.handleClient();
  // When using the motor, we must poll for position updates.
  checkMotor();
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
  //   "led-color": 100000,
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
  doc["led-color"] = getLedColor();

  JsonObject motor = doc.createNestedObject("motor");
  motor["speed"] = getMotorSpeed();
  motor["position"] = getMotorPosition();
  motor["acceleration"] = getMotorAcceleration();

  char output[128];
  serializeJson(doc, output);
  server.send(successStatusCode, contentTypeJson, output); 
}
