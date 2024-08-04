#include <AccelStepper.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "pinouts.h"
#include "server_essentials.h"

static const char* _motorJsonKey = "motor";
static const char* _positionJsonKey = "position";
static const char* _accelerationJsonKey = "acceleration";
static const char* _speedJsonKey = "speed";

static int16_t motorPosition = 0;
static const uint16_t _hardcodedMaxSpeed = 1000;
static const uint16_t _hardcodedMaxAcceleration = 1000;

// Define the stepper and the pins the will use:
// D7 - Step
// D8 - Direction
// Enable pin is optional. See API docs for implementation.
AccelStepper motor(AccelStepper::DRIVER, D7, D8);

void _handleMotorPosition();
void _handleMotorSpeed();
void _handleMotorAcceleration();

uint16_t getMotorSpeed();
uint16_t getMotorAcceleration();

void initialiseMotorManagement() {
  uint16_t motorSpeed = getMotorSpeed();
  uint16_t motorAcceleration = getMotorAcceleration();

  // Add a safety check to ensure the values are indeed below a safe threshold.
  if (motorSpeed > _hardcodedMaxSpeed) {
    motorSpeed = _hardcodedMaxSpeed;
    writeShortToEeprom(getMotorSpeedMemoryLocation(), motorSpeed);
  }
  if (motorAcceleration > _hardcodedMaxAcceleration) {
    motorAcceleration = _hardcodedMaxAcceleration;
    writeShortToEeprom(getMotorAccelerationMemoryLocation(), motorAcceleration);
  }
  motor.setMaxSpeed((long)getMotorSpeed());
  motor.setAcceleration((long)getMotorAcceleration());
}

void registerMotorManagement() {
  server.on("/motor/position", HTTP_POST, _handleMotorPosition);
  server.on("/motor/speed", HTTP_POST, _handleMotorSpeed);
  server.on("/motor/acceleration", HTTP_POST, _handleMotorAcceleration);
}

int16_t getMotorPosition() {
  return motorPosition;
}

uint16_t getMotorSpeed() {
  return getShortFromEeprom(getMotorSpeedMemoryLocation());
}

uint16_t getMotorAcceleration() {
  return getShortFromEeprom(getMotorAccelerationMemoryLocation());
}

static void _onPositionSuccess(JsonDocument* doc) {
  motorPosition = (*doc)[_positionJsonKey].as<int16_t>();
  // No need to write to EEPROM, as we're not storing this value. Just move the
  // motor to the desired position.
  Serial.println(motorPosition);
  motor.moveTo((long)motorPosition);
}

static void _onSpeedSuccess(JsonDocument* doc) {
  uint16_t speed = (*doc)[_speedJsonKey].as<uint16_t>();
  writeShortToEeprom(getMotorSpeedMemoryLocation(), speed);
  motor.setMaxSpeed((long)speed);
}

static void _onAccelerationSuccess(JsonDocument* doc) {
  uint16_t acceleration = (*doc)[_accelerationJsonKey].as<uint16_t>();
  writeShortToEeprom(getMotorAccelerationMemoryLocation(), acceleration);
  motor.setAcceleration((long)acceleration);
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