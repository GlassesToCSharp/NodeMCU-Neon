#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include "device_management.h"
#include "eeprom_handler.h"
#include "led_handler.h"
#include "pinouts.h"
#include "power_management.h"
#include "server_essentials.h"
#include "wifi_credentials.h"
 
void setup() {
  Serial.begin(115200);
  delay(10);
  // TODO: Initialise power state (default off)
  initialiseLedHandler();
  initialiseEeprom();
  // TODO: Initialise motors
  Serial.println(F("Testing light..."));
  delay(5000);
  setLedHandlerState(STATE_CONNECTING);

  // Load and initialise


  // TODO: Uncomment the below to read network details from EEPROM, and remove the header to wifi_credentials.
  // Read Network SSID and Passcode from EEPROM
  // char ssid[MAX_SSID_LENGTH];
  // char password[MAX_KEY_LENGTH];
  // getNetworkSsid(ssid);
  // getNetworkKey(password);

  if (ssid[0] == '\0') {
    Serial.println(F("No network SSID saved."));
    Serial.println(F("A network SSID must be saved to connect to WiFi."));
    while(1) {
      delay(1000);
    }
  }
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
    if (WiFi.status() == WL_CONNECT_FAILED) {
      setLedHandlerState(STATE_FAILED);
      setBoardLedState(false);
      while(1){
        delay(2000);
      }
    }
  }
  setLedHandlerState(STATE_CONNECTED);
  
  Serial.println();
  Serial.println(F("WiFi connected"));
 
  // Start the server
  server.begin();
  Serial.println(F("Server started"));
 
  // Print the IP address
  Serial.print(F("Use this URL to connect: http://"));
  Serial.print(WiFi.localIP());
  Serial.println(F("/"));
  
  // Handlers
  initialiseDeviceManagement();
  initialisePowerManagement();
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/neon-brightness", HTTP_POST, handleNeonBrightness);
  server.on("/motor", HTTP_POST, handleMotor);
  server.on("/save", HTTP_POST, handleSave);
}
 
void loop() {
  // Check if a client has connected
  server.handleClient();
}

//////////////////////

// Default values
byte neonBrightness = 0;
int motorAcceleration = 0;
int motorSpeed = 0;
int motorPosition = 0;


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
  doc["power"] = powerStatus;
  doc["neon"] = neonBrightness;

  JsonObject motor = doc.createNestedObject("motor");
  motor["speed"] = motorSpeed;
  motor["position"] = motorPosition;
  motor["acceleration"] = motorAcceleration;

  char output[128];
  serializeJson(doc, output);
  server.send(successStatusCode, contentTypeJson, output); 
}

void handleNeonBrightness() {
  int statusCode = failStatusCode;
  if (server.hasArg("plain")) {
    String json = server.arg("plain");
    JsonDocument doc;
    deserializeJson(doc, json);
    byte value = doc["brightness"];
    if (value >= 0 && value < 256) {
      statusCode = successEmptyStatusCode;
      // TODO: Set PWM of Neon brightness
      // Use analogWrite(NeonPin, value) ?
      neonBrightness = value;
    }
  }
  server.send(statusCode, contentTypePlain, emptyResponse); 
}

void handleMotor() {
  int statusCode = failStatusCode;
  if (server.hasArg("plain")) {
    String json = server.arg("plain");
    JsonDocument doc;
    deserializeJson(doc, json);
    if (doc.containsKey("acceleration")) {
      // TODO: Set motor acceleration
      motorAcceleration = doc["acceleration"];
    }
    if (doc.containsKey("speed")) {
      // TODO: Set motor speed
      motorSpeed = doc["speed"];
    }
    if (doc.containsKey("position")) {
      // TODO: Set motor position
      motorPosition = doc["position"];
    }
    statusCode = successEmptyStatusCode;
  }
  server.send(statusCode, contentTypePlain, emptyResponse); 
}

void handleSave() {
  // TODO: Write values to EEPROM
  // Save motor acceleration
  // Save motor speed
  // Save Neon brightness
  server.send(successEmptyStatusCode, contentTypePlain, emptyResponse); 
}
