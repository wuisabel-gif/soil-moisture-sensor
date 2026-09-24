#include "CapacitiveSoilMoisture.h"

#include <math.h>

CapacitiveSoilMoisture::CapacitiveSoilMoisture(CapacitiveSoilPin analogPin,
                                               int dryValue,
                                               int wetValue)
    : analogPin_(analogPin), dryValue_(dryValue), wetValue_(wetValue) {}

void CapacitiveSoilMoisture::begin() { pinMode(analogPin_, INPUT); }

int CapacitiveSoilMoisture::readRaw(uint16_t samples,
                                    uint16_t sampleDelayMs) const {
  if (samples == 0) {
    samples = 1;
  }

  uint32_t total = 0;
  for (uint16_t i = 0; i < samples; ++i) {
    total += static_cast<uint32_t>(analogRead(analogPin_));
    if (sampleDelayMs > 0 && i + 1 < samples) {
      delay(sampleDelayMs);
    }
  }

  // Adding half the divisor rounds to the nearest ADC count.
  return static_cast<int>((total + samples / 2U) / samples);
}

CapacitiveSoilMoisture::Reading CapacitiveSoilMoisture::read(
    uint16_t samples, uint16_t sampleDelayMs) const {
  const int raw = readRaw(samples, sampleDelayMs);
  const bool calibrated = hasValidCalibration();
  return {raw,
          calibrated ? calculatePercent(raw, dryValue_, wetValue_) : NAN,
          calibrated};
}

float CapacitiveSoilMoisture::readPercent(uint16_t samples,
                                          uint16_t sampleDelayMs) const {
  return read(samples, sampleDelayMs).percent;
}

void CapacitiveSoilMoisture::setCalibration(int dryValue, int wetValue) {
  dryValue_ = dryValue;
  wetValue_ = wetValue;
}

int CapacitiveSoilMoisture::dryValue() const { return dryValue_; }

int CapacitiveSoilMoisture::wetValue() const { return wetValue_; }

bool CapacitiveSoilMoisture::hasValidCalibration() const {
  return dryValue_ != wetValue_;
}

float CapacitiveSoilMoisture::calculatePercent(int raw, int dryValue,
                                               int wetValue) {
  if (dryValue == wetValue) {
    return NAN;
  }

  float percent = 100.0f * static_cast<float>(raw - dryValue) /
                  static_cast<float>(wetValue - dryValue);
  if (percent < 0.0f) {
    return 0.0f;
  }
  if (percent > 100.0f) {
    return 100.0f;
  }
  return percent;
}
