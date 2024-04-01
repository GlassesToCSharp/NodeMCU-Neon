#ifndef SERVER_ESSENTIALS_H
#define SERVER_ESSENTIALS_H

#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* contentTypePlain = "text/plain";
const char* contentTypeJson = "application/json";
const char* emptyResponse = "";
const int failStatusCode = 400;
const int successStatusCode = 200;
const int successEmptyStatusCode = 204;

#endif