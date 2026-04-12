#include <Arduino.h>

#include "eeprom_handler.h"
#include "eeprom_memory_management.h"
#include "feature_management.h"

uint8_t _features = 0x00; // Everything disabled

uint8_t _mapFeatureToByteLocation(const Feature feature);
void _updateFeatures(const uint8_t newFeatures);

void initialiseFeatureManagement() {
  initialiseEeprom();
  _features = getByteFromEeprom(getFeaturesMemoryLocation());
  // On a clean install, the EEPROM seems to hold a value of 0xFF. Clear it and
  // start afresh.
  if (_features == 0xFF) {
    _updateFeatures(0x00);
  }
}

bool isFeatureEnabled(const Feature feature) {
  // The idea is that "_features" contains the bit for each feature. It can be
  // found using logical operations. If the bit is high, the feature is enabled.
  // This is found by using the value and the AND operator to compare. 
  // For example:
  // _features = 0b0000 0010
  // featureMap1 = 0x01
  // featureMap2 = 0x02
  // _features & 0x01 == 0x00 == false
  // _features & 0x02 == 0x02 == true
  Serial.print("Features: "); Serial.println(_features, HEX);
  uint8_t mapping = _mapFeatureToByteLocation(feature);
  Serial.print("Mapping: "); Serial.println(mapping, HEX);
  Serial.print("Result: "); Serial.println(_features & mapping, HEX);
  return _features & mapping;
}

void enableFeature(const Feature feature) {
  // OR operator raises the bit high to enable the feature.
  uint8_t mapping = _mapFeatureToByteLocation(feature);
  uint8_t newFeatures = _features | mapping;
  _updateFeatures(newFeatures);
}

void disableFeature(const Feature feature) {
  // And operator with the logical inverse lowers the bit to disable the feature.
  uint8_t mapping = _mapFeatureToByteLocation(feature);
  uint8_t newFeatures = _features & ~mapping;
  _updateFeatures(newFeatures);
}

void _updateFeatures(const uint8_t newFeatures) {
  if (_features != newFeatures) {
    _features = newFeatures;
    writeByteToEeprom(getFeaturesMemoryLocation(), _features);
  }
}

uint8_t _mapFeatureToByteLocation(const Feature feature) {
  switch(feature) {
    case LED_STRIP: return 0x01;
    case NEON: return 0x01 << 1;
    case GENERAL_POWER: return 0x01 << 2;
    case MOTOR: return 0x01 << 3;
  }

  return 0xFF;
}
