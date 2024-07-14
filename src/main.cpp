//#define USESERIAL//You need to switch the platform.ini build flags to use this
#define USETOUCHHID // Define to enable touch HID functionality

#include <Arduino.h>
#include <USBHost_t36.h>
#include "PixcirTouchPanel.h"

// Create USBHost and USBHIDParser objects
USBHost myusb;
USBHIDParser hid1(myusb);

// Create PixcirTouchPanel object and pass the HID parser to it
PixcirTouchPanel myTouchPanel(hid1);

void setup() {
    #ifdef USESERIAL
    // Initialize serial communication for debugging
    Serial.begin(115200);
    while (!Serial); // Wait for the serial monitor to open
    #endif
    
    // Initialize the USB host
    myusb.begin();

    #ifdef USESERIAL
    // Print initialization message to Serial if USESERIAL is defined
    Serial.println("USB Host initialized. Waiting for devices...");
    #endif

    #ifdef USETOUCHHID
    // Initialize the touchscreen HID if USETOUCHHID is defined
    TouchscreenUSB.begin();
    #endif
}

void loop() {
    // Run the USB task to handle USB events
    myusb.Task();
}
