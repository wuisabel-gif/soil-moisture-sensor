#include <CapacitiveSoilMoisture.h>

constexpr uint8_t kSoilPin = A0;

// Replace these example values after running the Calibration example.
CapacitiveSoilMoisture soil(kSoilPin, 812, 347);

void setup() {
  Serial.begin(115200);
  soil.begin();
}

void loop() {
  const CapacitiveSoilMoisture::Reading reading = soil.read();

  Serial.print("Raw: ");
  Serial.print(reading.raw);
  Serial.print("  Moisture: ");
  if (reading.calibrated) {
    Serial.print(reading.percent, 1);
    Serial.println('%');
  } else {
    Serial.println("not calibrated");
  }

  delay(1000);
}
