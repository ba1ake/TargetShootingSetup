#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// NRF24L01 pins (CE, CSN)
RF24 radio(9, 10); 

const byte address[6] = "00001"; // Address to match with base station
const int vibrationPin = 2;      // Vibration sensor connected to D2

// Unique ID for this target (Target 1, Target 2, etc.)
int targetID = 1;  

void setup() {
  pinMode(vibrationPin, INPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
}

void loop() {
  int vibrationState = digitalRead(vibrationPin);

  if (vibrationState == HIGH) {  // Hit detected!
    char hitMessage[32];
    snprintf(hitMessage, sizeof(hitMessage), "HIT %d", targetID); // Send ID and "HIT"
    radio.write(&hitMessage, sizeof(hitMessage));
    Serial.println("Hit sent!");

    delay(500); // Simple debounce to prevent double hits
  }
}
