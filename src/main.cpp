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
  
  initialiseFeatureManagement();
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
  // Handle the UDP broadcast message
  checkForUdpBroadcastMessage();
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
  // Size is determined using the ArduinJson Assistant (not really required for v7):
  // https://arduinojson.org/v7/assistant/#/step1
  // JSON structure:
  // {
  //   "name": "Device Name        ", // Max size: 19 chars long (20 including '\0' char)
  //   "power": false,
  //   "neon": 255, UINT8
  //   "led-color": 16777215, UINT32 (24-bits)
  //   "motor": {
  //     "speed": 1234,
  //     "position": 1234,
  //     "acceleration": 1234
  //   }
  // }
  JsonDocument doc;

  char deviceName[20];
  getDeviceName(deviceName);
  doc["name"] = deviceName;
  if (isFeatureEnabled(GENERAL_POWER)) {
    doc["power"] = getPowerStatus();
  }
  if (isFeatureEnabled(NEON)) {
    doc["neon"] = getNeonBrightness();
  }
  if (isFeatureEnabled(LED_STRIP)) {
    doc["led-color"] = getLedColor();
  }

  if (isFeatureEnabled(MOTOR)) {
    JsonObject motor = doc["motor"].to<JsonObject>();
    motor["speed"] = getMotorSpeed();
    motor["position"] = getMotorPosition();
    motor["acceleration"] = getMotorAcceleration();
  }

  doc.shrinkToFit();

  String output;
  serializeJson(doc, output);
  server.send(successStatusCode, contentTypeJson, output); 
}
