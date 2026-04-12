#ifndef FEATURE_MANAGEMENT_H
#define FEATURE_MANAGEMENT_H

#include <Arduino.h>

enum Feature {
    LED_STRIP,
    NEON,
    GENERAL_POWER,
    MOTOR
};

const uint8_t FEATURES_MAX_LENGTH = 1;
static const char* jsonKeyFeatureStatus = "feature_status";

extern void initialiseFeatureManagement();
extern void enableFeature(const Feature feature);
extern void disableFeature(const Feature feature);
extern bool isFeatureEnabled(const Feature feature);

#endif