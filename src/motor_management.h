#ifndef MOTOR_MANAGEMENT_H
#define MOTOR_MANAGEMENT_H

#include <AccelStepper.h>
#include <Arduino.h>

const uint8_t MOTOR_SPEED_MAX_LENGTH = 2;
const uint8_t MOTOR_ACCELERATION_MAX_LENGTH = 2;


extern const char* const jsonKeyMotor;
extern const char* const jsonKeyPosition;
extern const char* const jsonKeyAcceleration;
extern const char* const jsonKeySpeed;

extern void initialiseMotorManagement();
extern void registerMotorManagement();
extern int16_t getMotorPosition();
extern uint16_t getMotorSpeed();
extern uint16_t getMotorAcceleration();
extern void checkMotor();

#endif