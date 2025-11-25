#include <DigiKeyboard.h>

/* 
This firmware program is compatible for VBand CW Paddle Mode
Modified for ATtiny85 microcontroller
Website URL: http://oz1jhm.dk/content/hamradio-solutions-vband-interface
*/

// PINS
const int DIT_PIN = 0;
const int DAH_PIN = 2;
const int LED = 1;

void setup() {
  // Initialize the digital LED pin as an output
  pinMode(LED, OUTPUT);  // LED on Model A

  // Initialize the DIT and DAH pin mode
  pinMode(DIT_PIN, INPUT_PULLUP);
  pinMode(DAH_PIN, INPUT_PULLUP);
}

// Debounce
void loop() {
 while (digitalRead(DIT_PIN) == HIGH && digitalRead(DAH_PIN) == HIGH){

    DigiKeyboard.sendKeyPress(0);
    digitalWrite(LED, LOW);

  }

  if ( digitalRead(DIT_PIN) == LOW){

    DigiKeyboard.sendKeyPress(0, MOD_CONTROL_LEFT);
    digitalWrite(LED, HIGH);

  }

  else{

    DigiKeyboard.sendKeyPress(0);
    digitalWrite(LED, LOW);

  }

  

  if ( digitalRead(DAH_PIN) == LOW){

    DigiKeyboard.sendKeyPress(0, MOD_CONTROL_RIGHT);
    digitalWrite(LED, HIGH);

  }

  else{

    DigiKeyboard.sendKeyPress(0);
    digitalWrite(LED, LOW);

  }

  DigiKeyboard.delay(5);
}