#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

static const char* jsonKeyPower = "power";

extern void initialisePowerManagement();
extern void registerPowerManagement();
extern bool getPowerStatus();

#endif