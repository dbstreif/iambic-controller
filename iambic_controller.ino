#include <DigiKeyboard.h>

/* 
This firmware program is compatible with VBand CW Iambic Paddle Modes (Should work for many other other programs as well)
Modified for ATtiny85 microcontroller
Website URL (for inspiration, but found that it was bad logic): http://oz1jhm.dk/content/hamradio-solutions-vband-interface
Author: Dominic Streif [https://github.com/dbstreif]
*/

// PINS
const int DIT_PIN = 0;
const int DAH_PIN = 2;
const int LED = 1;

// Debounce time
const unsigned long DEBOUNCE = 8;

// Setup time parameters
unsigned long timeDit = 0;
unsigned long timeDah = 0;

// Setup prevState parameters
int lastDitState = HIGH;
int lastDahState = HIGH;

// Setup modifier state (enables press/release entangled keyboard modifier states)
unsigned int modifierState = 0;

// Setup modifier types
unsigned int ditMOD = MOD_CONTROL_LEFT;
unsigned int dahMOD = MOD_CONTROL_RIGHT;

void setup() {
  // Initialize the digital LED pin as an output
  pinMode(LED, OUTPUT);  // LED on Model A

  // Initialize the DIT and DAH pin mode
  pinMode(DIT_PIN, INPUT_PULLUP);
  pinMode(DAH_PIN, INPUT_PULLUP);
}


// Function for sending keypress
unsigned long sendKey(int pin, unsigned long lastTime, int *lastState, unsigned int modtype, unsigned int *modState) {
  unsigned long nowTime = millis();
  
  int curState = digitalRead(pin);

  if ((nowTime - lastTime >= DEBOUNCE) && (curState != *lastState)) {
    // Race condition requirement: Set state and time immediately after debounce check
    *lastState = curState;
    lastTime = nowTime;

    // Applies/Removes modifier states with bitwise operations (allows press/release after alternating sequence without releasing all keypresses)
    if (!curState) {
      *modState |= modtype;
    } else {
      *modState &= ~modtype;
    }

    // Send Keypress to HID input
    DigiKeyboard.sendKeyPress(0, *modState);
  }
  
  return lastTime;
}


// main loop polling pin states
void loop() {
  timeDit = sendKey(DIT_PIN, timeDit, &lastDitState, ditMOD, &modifierState);
  timeDah = sendKey(DAH_PIN, timeDah, &lastDahState, dahMOD, &modifierState);

  // LED logic
  if (!digitalRead(DIT_PIN) || !digitalRead(DAH_PIN)) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  DigiKeyboard.delay(1);
}
