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
  if (isFeatureEnabled(MOTOR)) {
    checkMotor();
  }

  // Do colour fade if it is enabled
  if (isFeatureEnabled(LED_STRIP)){
    fadeColor();
  }

  // Do neon fade if it is enabled
  if (isFeatureEnabled(NEON)){
    fadeNeon();
  }
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
  //   "id": "XXXX",
  //   "name": "Device Name        ", // Max size: 19 chars long (20 including '\0' char)
  //   "signal": 255, INT8
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

  char deviceId[DEVICE_ID_MAX_LENGTH];
  getDeviceId(deviceId);
  doc[jsonKeyId] = deviceId;
  char deviceName[DEVICE_NAME_MAX_LENGTH];
  getDeviceName(deviceName);
  doc[jsonKeyName] = deviceName;
  doc[jsonKeySignal] = getSignalStrength();
  if (isFeatureEnabled(GENERAL_POWER)) {
    doc[jsonKeyPower] = getPowerStatus();
  }
  if (isFeatureEnabled(NEON)) {
    doc[jsonKeyNeon] = getNeonBrightness();
  }
  if (isFeatureEnabled(LED_STRIP)) {
    doc[jsonKeyColor] = getLedColor();
  }

  if (isFeatureEnabled(MOTOR)) {
    JsonObject motor = doc[jsonKeyMotor].to<JsonObject>();
    motor[jsonKeySpeed] = getMotorSpeed();
    motor[jsonKeyPosition] = getMotorPosition();
    motor[jsonKeyAcceleration] = getMotorAcceleration();
  }

  doc.shrinkToFit();

  String output;
  serializeJson(doc, output);
  server.send(successStatusCode, contentTypeJson, output); 
}
