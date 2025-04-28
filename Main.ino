#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// NRF24L01 pins (CE, CSN)
RF24 radio(9, 10);

const byte address[6] = "00001"; // Same address as the targets

// Times for each target to "activate" (in milliseconds)
unsigned long courseSchedule[] = {10000, 20000, 30000};  // Target 1 at 10s, Target 2 at 20s, Target 3 at 30s

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
}

void loop() {
  unsigned long currentTime = millis();

  // Activate targets based on the schedule
  for (int i = 0; i < 3; i++) {
    if (currentTime >= courseSchedule[i]) {
      char activationMessage[32];
      snprintf(activationMessage, sizeof(activationMessage), "ACTIVATE %d", i + 1);  // Target ID
      radio.write(&activationMessage, sizeof(activationMessage));
      Serial.print("Target ");
      Serial.print(i + 1);
      Serial.println(" activated!");
      courseSchedule[i] = ULONG_MAX; // Ensure it only activates once
    }
  }

  if (radio.available()) {
    char hitMessage[32] = {0};
    radio.read(&hitMessage, sizeof(hitMessage));

    int targetID;
    sscanf(hitMessage, "HIT %d", &targetID);

    unsigned long timestamp = millis(); // Time in milliseconds
    Serial.print("Target ");
    Serial.print(targetID);
    Serial.print(" hit at ");
    Serial.print(timestamp);
    Serial.println(" ms");
  }
}
