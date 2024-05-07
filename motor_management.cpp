#include <Arduino.h>
#include <ArduinoJson.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "server_essentials.h"

static const char* _motorJsonKey = "motor";
static const char* _positionJsonKey = "position";
static const char* _accelerationJsonKey = "acceleration";
static const char* _speedJsonKey = "speed";

static uint16_t motorPosition = 0;

void _handleMotorPosition();
void _handleMotorSpeed();
void _handleMotorAcceleration();

void initialiseMotorManagement() {
  // TODO: Setup Stepper Motor with library
}

void registerMotorManagement() {
  server.on("/motor/position", HTTP_POST, _handleMotorPosition);
  server.on("/motor/speed", HTTP_POST, _handleMotorSpeed);
  server.on("/motor/acceleration", HTTP_POST, _handleMotorAcceleration);
}

uint16_t getMotorPosition() {
  return motorPosition;
}

uint16_t getMotorSpeed() {
  return getIntFromEeprom(getMotorSpeedMemoryLocation());
}

uint16_t getMotorAcceleration() {
  return getIntFromEeprom(getMotorAccelerationMemoryLocation());
}

static void _onPositionSuccess(JsonDocument* doc) {
  motorPosition = (*doc)[_positionJsonKey].as<uint16_t>();
  // No need to write to EEPROM, as we're not storing this value. Just move the
  // motor to the desired position.
  // TODO: Set the motor position using the motor library
}

static void _onSpeedSuccess(JsonDocument* doc) {
  uint16_t speed = (*doc)[_speedJsonKey].as<uint16_t>();
  writeIntToEeprom(getMotorSpeedMemoryLocation(), speed);
  // TODO: Set the motor speed using the motor library
}

static void _onAccelerationSuccess(JsonDocument* doc) {
  uint16_t acceleration = (*doc)[_accelerationJsonKey].as<uint16_t>();
  writeIntToEeprom(getMotorAccelerationMemoryLocation(), acceleration);
  // TODO: Set the motor acceleration using the motor library
}

void _handleMotorPosition() {
  handleHttpPost(_positionJsonKey, _onPositionSuccess);
}

void _handleMotorSpeed() {
  handleHttpPost(_speedJsonKey, _onSpeedSuccess);
}

void _handleMotorAcceleration() {
  handleHttpPost(_accelerationJsonKey, _onAccelerationSuccess);
}