#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "eeprom_handler.h"
#include "led_handler.h"
#include "pinouts.h"
#include "wifi_credentials.h"

int espLedPin = WIFI_LED; // ESP LED Pin
int boardLedPin = BOARD_LED; // Board LED Pin

ESP8266WebServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
  initialiseLedHandler();
  Serial.println(F("Testing light..."));
  delay(5000);
  setLedHandlerState(STATE_CONNECTING);

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
  server.on("/power", handlePowerState);
  server.on("/neon-brightness", handleNeonBrightness);
}
 
void loop() {
  // Check if a client has connected
  server.handleClient();
}

void handlePowerState() {
  bool value = false;
  int statusCode = 400;
  char content = "text/plain";
  char response[20] = "";
  if (server.hasArg("plain")) {
    String json = server.arg("plain");
    JsonDocument doc;
    deserializeJson(doc, json);
    value = doc["state"];
    statusCode = 204;
    // TODO: Set power state of components
    // Use digitalWrite(PowerPin) ?
  }
  server.send(statusCode, content, response); 
}

void handleNeonBrightness() {
  byte value = 0;
  int statusCode = 400;
  char content = "text/plain";
  char response[20] = "";
  if (server.hasArg("plain")) {
    String json = server.arg("plain");
    JsonDocument doc;
    deserializeJson(doc, json);
    value = doc["brightness"];
    if (value < 0 || value > 255) {
      response = F("0<=Brightess<=20")
    } else {
      statusCode = 204;
      // TODO: Set PWN of Neon brightness
      // Use analogWrite(NeonPin) ?
    }
  }
  server.send(statusCode, content, response); 
}
