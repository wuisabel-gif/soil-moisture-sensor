#include <assert.h>
#include <math.h>

#include <initializer_list>
#include <vector>

#include "CapacitiveSoilMoisture.h"

namespace {
std::vector<int> samples;
size_t sampleIndex = 0;
uint8_t configuredPin = 255;
unsigned long delayCalls = 0;
}  // namespace

void pinMode(uint8_t pin, uint8_t mode) {
  assert(mode == INPUT);
  configuredPin = pin;
}

int analogRead(uint8_t pin) {
  assert(pin == configuredPin);
  assert(sampleIndex < samples.size());
  return samples[sampleIndex++];
}

void delay(unsigned long) { ++delayCalls; }

void loadSamples(std::initializer_list<int> values) {
  samples = values;
  sampleIndex = 0;
  delayCalls = 0;
}

int main() {
  CapacitiveSoilMoisture sensor(3, 800, 300);
  sensor.begin();
  assert(configuredPin == 3);

  assert(CapacitiveSoilMoisture::calculatePercent(800, 800, 300) == 0.0f);
  assert(CapacitiveSoilMoisture::calculatePercent(300, 800, 300) == 100.0f);
  assert(CapacitiveSoilMoisture::calculatePercent(550, 800, 300) == 50.0f);
  assert(CapacitiveSoilMoisture::calculatePercent(900, 800, 300) == 0.0f);
  assert(CapacitiveSoilMoisture::calculatePercent(200, 800, 300) == 100.0f);
  assert(CapacitiveSoilMoisture::calculatePercent(500, 300, 700) == 50.0f);
  assert(isnan(CapacitiveSoilMoisture::calculatePercent(500, 400, 400)));

  loadSamples({500, 501, 502, 503});
  assert(sensor.readRaw(4, 5) == 502);
  assert(delayCalls == 3);

  loadSamples({550});
  const CapacitiveSoilMoisture::Reading reading = sensor.read(0, 0);
  assert(reading.raw == 550);
  assert(reading.percent == 50.0f);
  assert(reading.calibrated);

  sensor.setCalibration(42, 42);
  loadSamples({42});
  const CapacitiveSoilMoisture::Reading uncalibrated = sensor.read(1, 0);
  assert(!uncalibrated.calibrated);
  assert(isnan(uncalibrated.percent));
}
