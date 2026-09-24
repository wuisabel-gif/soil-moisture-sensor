#pragma once

// Particle Device OS provides the Arduino-style GPIO API through Particle.h.
// Arduino cores provide the same API through Arduino.h.
#if defined(PARTICLE) || defined(SPARK) || defined(PLATFORM_ID)
#include "Particle.h"
using CapacitiveSoilPin = pin_t;
#else
#include <Arduino.h>
using CapacitiveSoilPin = uint8_t;
#endif

class CapacitiveSoilMoisture {
 public:
  struct Reading {
    int raw;
    float percent;
    bool calibrated;
  };

  CapacitiveSoilMoisture(CapacitiveSoilPin analogPin, int dryValue,
                         int wetValue);

  void begin();

  // Returns the arithmetic mean of one or more ADC samples. A sample count of
  // zero is treated as one sample.
  int readRaw(uint16_t samples = 10, uint16_t sampleDelayMs = 5) const;

  // Takes one averaged reading and returns both its raw and calibrated forms.
  Reading read(uint16_t samples = 10, uint16_t sampleDelayMs = 5) const;

  // Returns NAN when the dry and wet calibration endpoints are equal.
  float readPercent(uint16_t samples = 10,
                    uint16_t sampleDelayMs = 5) const;

  void setCalibration(int dryValue, int wetValue);
  int dryValue() const;
  int wetValue() const;
  bool hasValidCalibration() const;

  // Converts a raw ADC reading to 0-100%. Values outside the calibrated range
  // are clamped. This also supports sensors whose readings rise when wet.
  static float calculatePercent(int raw, int dryValue, int wetValue);

 private:
  CapacitiveSoilPin analogPin_;
  int dryValue_;
  int wetValue_;
};
