#ifndef MOTOR_MANAGEMENT_H
#define MOTOR_MANAGEMENT_H

#include <Arduino.h>

const uint8_t MOTOR_SPEED_MAX_LENGTH = 2;
const uint8_t MOTOR_ACCELERATION_MAX_LENGTH = 2;

extern void initialiseMotorManagement();
extern uint16_t getMotorPosition();
extern uint16_t getMotorSpeed();
extern uint16_t getMotorAcceleration();

#endif