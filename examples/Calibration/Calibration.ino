#include <CapacitiveSoilMoisture.h>

constexpr uint8_t kSoilPin = A0;

// Calibration endpoints are irrelevant while only readRaw() is used.
CapacitiveSoilMoisture soil(kSoilPin, 0, 1);

void setup() {
  Serial.begin(115200);
  soil.begin();
  Serial.println("Record the stable raw value in dry soil, then in saturated soil.");
}

void loop() {
  Serial.println(soil.readRaw(20, 10));
  delay(500);
}
