#include <DigiCDC.h>

// PINS
const int DIT_PIN = 0;
const int DAH_PIN = 2;
const int LED = 1;

// Paddle Debounce
const unsigned long DEBOUNCE_MS = 5;

// Vars
unsigned long now = 0;
unsigned long lastDitChange = 0;
unsigned long lastDahChange = 0;
bool currentDit = 1, currentDah = 1;
bool lastDit = 1, lastDah = 1;

int ditState = 0;
int dahState = 0;

void setup() {
  // Initialize the digital LED pin as an output
  pinMode(LED, OUTPUT);  // LED on Model A

  // Initialize the DIT and DAH pin mode
  pinMode(DIT_PIN, INPUT_PULLUP);
  pinMode(DAH_PIN, INPUT_PULLUP);

  // Initialize the Serializer
  SerialUSB.begin();
  // 5 Second Start Delay to let VSerial reader attach
  SerialUSB.delay(2000);
  SerialUSB.refresh();
  SerialUSB.println(F("Firmware fully booted!"));
}

// Debounce
void loop() {
  now = millis();

  // ---- DIT ----
  currentDit = digitalRead(DIT_PIN);
  if (currentDit != lastDit && (now - lastDitChange >= DEBOUNCE_MS)) {
    lastDitChange = now;
    lastDit = currentDit;
    ditState = !ditState;
    SerialUSB.print(ditState);
    SerialUSB.println(dahState);
  }

  // ---- DAH ----
  currentDah = digitalRead(DAH_PIN);
  if (currentDah != lastDah && (now - lastDahChange >= DEBOUNCE_MS)) {
    lastDahChange = now;
    lastDah = currentDah;
    dahState = !dahState;
    SerialUSB.print(ditState);
    SerialUSB.println(dahState);
  }


  // LED indicates any paddle pressed
  digitalWrite(LED, ditState || dahState);

  SerialUSB.task(); // keeps USB alive
}

/*
// Paddle hold
void loop() {
  bool dit = digitalRead(DIT_PIN) == LOW;
  bool dah = digitalRead(DAH_PIN) == LOW;

  // send while held
  if (dit && dah) {
    SerialUSB.println("DIT DAH");
    digitalWrite(LED, HIGH);
    SerialUSB.delay((1200 / WPM) + ((1200 / WPM) * 3));
  } else if (dit) {
    SerialUSB.println("DIT");
    digitalWrite(LED, HIGH);
    SerialUSB.delay(1200 / WPM);
  } else if (dah) {
    SerialUSB.println("DAH");
    digitalWrite(LED, HIGH);
    SerialUSB.delay((1200 / WPM) * 3);
  } else {
    digitalWrite(LED, LOW);
    SerialUSB.delay(5);
  }

  SerialUSB.task();      // keep USB alive
}
*/
