# Iambic Controller DigiStump ATtiny85 USB Microcontroller Firmware
### (Morse Code Keyer Software)

A lightweight firmware for the DigiStump ATtiny85 that implements an iambic CW keyer controller over USB. 
This project provides paddle input handling, configurable keyer logic, and USB HID support.

To build and install on your microcontroller, please use the Arduino IDE with the Digistump additional boards manager:
[Digistump Additional Boards Manager](https://raw.githubusercontent.com/digistump/arduino-boards-index/master/package_digistump_index.json)

Additionally, this software can be flashed to any other arduino board, microcontroller, etc.
Simply use the approapriate HID Keyboard library (ex. Arduino's Keyboard library rather than DigiStump's)

### Hardware Instructions
1. Solder/Connect Dit paddle to PIN0 and Dah paddle to PIN2
2. Connect Ground wires to GND PIN
3. If modifying for another device, ensure you make the appropriate changes to pin configuration in iambic_controller.ino
4. Use a LED to test if PULLUP (Dit/Dah press/release) is working correctly.

### Compatibility
Plug and play with VBand, MorseIT (IOS/Android), MorseKeyer Software (PC)



![IMG_8415](https://github.com/user-attachments/assets/8373d34d-42c8-48fa-b347-3db874731b60)


Shield: [![CC BY 4.0][cc-by-shield]][cc-by]

This work is licensed under a
[Creative Commons Attribution 4.0 International License][cc-by].

[![CC BY 4.0][cc-by-image]][cc-by]

[cc-by]: http://creativecommons.org/licenses/by/4.0/
[cc-by-image]: https://i.creativecommons.org/l/by/4.0/88x31.png
[cc-by-shield]: https://img.shields.io/badge/License-CC%20BY%204.0-lightgrey.svg
