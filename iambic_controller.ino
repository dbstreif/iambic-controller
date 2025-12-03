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

// Setup toggle states
typedef struct {
  int toggle; // Will be 0 or 1
  int states[2]; // Will map to desired keypress
} ModeToggle;

ModeToggle ditm = {
  .toggle = 0,
  .states = { 0, MOD_CONTROL_LEFT } // State index 0 is redundant, however we keep it in case of needing a null send
};

ModeToggle dahm = {
  .toggle = 0,
  .states = { 0, MOD_CONTROL_RIGHT } // State index 0 is redundant, however we keep it in case of needing a null send
};


void setup() {
  // Initialize the digital LED pin as an output
  pinMode(LED, OUTPUT);  // LED on Model A

  // Initialize the DIT and DAH pin mode
  pinMode(DIT_PIN, INPUT_PULLUP);
  pinMode(DAH_PIN, INPUT_PULLUP);
}


// Function for sending keypress
unsigned long sendKey(int pin, unsigned long lastTime, int *lastState, ModeToggle *m, unsigned int *modState) {
  unsigned long nowTime = millis();
  
  int curState = digitalRead(pin);

  if ((nowTime - lastTime >= DEBOUNCE) && (curState != *lastState)) {
    // Race condition requirement: Set state and time immediately after debounce check
    *lastState = curState;
    lastTime = nowTime;

    // Applies/Removes modifier states with bitwise operations (allows press/release after alternating sequence without releasing all keypresses)
    if (!curState) {
      *modState |= m->states[1];
    } else {
      *modState &= ~m->states[1];
    }

    // Send Keypress to HID input
    DigiKeyboard.sendKeyPress(0, *modState);

    // Toggle mode
    m->toggle = !m->toggle;
  }
  
  return lastTime;
}


// main loop polling pin states
void loop() {
  timeDit = sendKey(DIT_PIN, timeDit, &lastDitState, &ditm, &modifierState);
  timeDah = sendKey(DAH_PIN, timeDah, &lastDahState, &dahm, &modifierState);

  // LED logic
  if (!digitalRead(DIT_PIN) || !digitalRead(DAH_PIN)) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  DigiKeyboard.delay(1);
}
