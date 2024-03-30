#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

#include "http_handler.h"
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
  Serial.println("Testing light...");
  delay(5000);
  setLedHandlerState(STATE_CONNECTING);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (WiFi.status() == WL_CONNECT_FAILED) {
      setLedHandlerState(STATE_FAILED);
      setBoardLedState(false);
      while(1){
        delay(2000);
      }
    }
  }
  setLedHandlerState(STATE_CONNECTED);
  
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
  server.on("/", handleOnConnect);
  server.on("/led=on", handleLedOnState);
  server.on("/led=off", handleLedOffState);
}
 
void loop() {
  // Check if a client has connected
  server.handleClient();
}

void handleOnConnect() {
  digitalWrite(boardLedPin, HIGH);
  server.send(200, "text/html", sendHtml(!digitalRead(boardLedPin))); 
}

void handleLedOnState() {
  digitalWrite(boardLedPin, LOW);
  server.send(200, "text/html", sendHtml(!digitalRead(boardLedPin))); 
}

void handleLedOffState() {
  digitalWrite(boardLedPin, HIGH);
  server.send(200, "text/html", sendHtml(!digitalRead(boardLedPin))); 
}

String sendHtml(bool status) {
  String ptr = "<!DOCTYPE HTML>";
  ptr += "<html>";
  ptr += "Led pin is now: ";
 
  if(status == HIGH) {
    ptr += "On";
  } else {
    ptr += "Off";
  }
  ptr += "<br><br>";
  ptr += "<a href=\"/led=on\"\"><button>Turn On </button></a>";
  ptr += "<a href=\"/led=off\"\"><button>Turn Off </button></a><br />";
  ptr += "</html>";

  return ptr;
}
 
