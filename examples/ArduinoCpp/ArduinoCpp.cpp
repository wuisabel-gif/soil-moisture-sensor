#include <Arduino.h>
#include <CapacitiveSoilMoisture.h>

namespace {
constexpr uint8_t kSoilPin = A0;

// Examples only. Replace these with values measured by Calibration.ino.
CapacitiveSoilMoisture soil(kSoilPin, 812, 347);
}  // namespace

void setup() {
  Serial.begin(115200);
  soil.begin();
}

void loop() {
  const CapacitiveSoilMoisture::Reading reading = soil.read();

  Serial.print("Raw: ");
  Serial.print(reading.raw);
  Serial.print("  Moisture: ");
  Serial.print(reading.percent, 1);
  Serial.println('%');

  delay(1000);
}
