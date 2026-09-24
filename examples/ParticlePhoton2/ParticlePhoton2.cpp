#include "Particle.h"
#include "CapacitiveSoilMoisture.h"

SYSTEM_THREAD(ENABLED);

namespace {
// A0 is a dedicated Photon 2 ADC pin. A1, A2, and A5 are also dedicated ADC
// pins; A3/D0 and A4/D1 are shared with I2C.
constexpr pin_t kSoilPin = A0;

// Examples only. Replace these with readings measured from your own probe.
CapacitiveSoilMoisture soil(kSoilPin, 3000, 1500);
}  // namespace

void setup() {
  Serial.begin(115200);
  soil.begin();
}

void loop() {
  const CapacitiveSoilMoisture::Reading reading = soil.read(10, 5);

  Serial.printlnf("Raw: %d  Moisture: %.1f%%", reading.raw,
                  static_cast<double>(reading.percent));
  delay(1000);
}
