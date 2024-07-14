# **Pixcir Touch Panel Reverse Engineering for NordicTrack S22i Bike**
This repository contains the code to reverse engineer the Pixcir touch panel used in the NordicTrack Bike S22i Commercial. The goal is to interface the touch panel with other hardware, such as a Raspberry Pi, using the USB host feature of the Teensy 4.0/4.1.

## Overview
The PixcirTouchPanel class in this project is designed to handle touch panel input from USB HID devices. By breaking off the D+/D- serial lines and 5V/GND from the touch screen interface and connecting them directly to the USB host, you can use this code to read touch input data. The primary purpose is to invert the X and Y axes to be the correct orientation by default and therefore does not require specific drivers on any hardware.

## Hardware Setup
- Identify the D+/D- serial lines and 5V/GND on the touch screen interface Pixcir Touch Tang C controller (USB not I2C).
- Connect these lines to a USB host adapter (e.g., USBHost_t36 for Teensy).
- Connect the Micro USB to the SBC/Computer of your choosing. Tested working with Android 14 with 5 point touch conversion.

## Software Setup
### Prerequisites
- PlatformIO in VSCode
- Teensyduino add-on for Arduino IDE (if using Teensy)
- A suitable USB host library (e.g., USBHost_t36)

## Installation
- Clone this repository.
- Open the workspace in VSCode.
- Wait for the VSCode project to set up.

## Usage
- Uncomment #define USESERIAL in main.ino if you want to enable serial debugging. Note that you need to switch the platform.ini build flags to use this.
- Compile and upload the sketch to your microcontroller - set up for the Teensy 4.0 or Teensy 4.1
- Open the Serial Monitor (if USESERIAL is defined) to view touch panel data.

## Code Explanation
PixcirTouchPanel.h
Defines and implements the PixcirTouchPanel class that handles touch input data from the USB HID device.
- claim_collection: Claims the HID collection if it is a touch panel.
- hid_input_data: Handles individual input data items (e.g., touch coordinates).
- hid_input_end: Processes the complete input report.
- disconnect_collection: Handles device disconnection.

main.cpp
Initializes and runs the USB host and touch panel:

- setup(): Initializes serial communication (if enabled), the USB host, and the touch HID.
- loop(): Runs the USB task to handle events.

## Contributing
Feel free to submit issues or pull requests to improve this project.

## License
For this project, [AGPL-3.0](https://choosealicense.com/licenses/agpl-3.0/) is used for licensing as a means to make sure any contributions or usage of the software can benefit the community. If you use and modify this software for a product, you must make the modified code readily available as per the license agreement.