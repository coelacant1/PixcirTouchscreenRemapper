#pragma once

#include <Arduino.h>
#include <USBHost_t36.h>

// PixcirTouchPanel class inherits from USBHIDInput to handle touch panel input from USB HID devices
class PixcirTouchPanel : public USBHIDInput {
public:
    // Constructor that initializes the touch panel and registers it with the HID parser
    PixcirTouchPanel(USBHIDParser &parser) : USBHIDInput() {
        parser.driver_ready_for_hid_collection(this);
    }

protected:
    // Method to claim the HID collection if it is a touch panel (Digitizer)
    virtual hidclaim_t claim_collection(USBHIDParser *driver, Device_t *dev, uint32_t topusage) override {
        // Check if the usage page is for Digitizers (0x000D)
        if ((topusage >> 16) == 0x000D) {
            //Serial.println("Touch panel claimed");
            return CLAIM_INTERFACE;
        }
        return CLAIM_NO;
    }

    // Method called at the beginning of an input report
    virtual void hid_input_begin(uint32_t topusage, uint32_t type, int lgmin, int lgmax) override {
        // Reset data for new input report
    }

    // Method called for each input data item in the report
    virtual void hid_input_data(uint32_t usage, int32_t value) override {
        #ifdef USESERIAL
        // Print usage and value to Serial if USESERIAL is defined
        Serial.print("Usage: ");
        Serial.print(usage, HEX);
        Serial.print(", Value: ");
        Serial.println(value);
        #endif
        
        // Handle different usages (Contact ID, X coordinate, Y coordinate)
        switch (usage) {
            case 0xD0051: // Contact ID (or finger ID)
                if (value < maxTouches) {
                    currentTouchID = value;
                    touchPoints[currentTouchID].active = true;
                }
                break;
            case 0x10031: // X coordinate
                if (currentTouchID >= 0 && currentTouchID < maxTouches) {
                    touchPoints[currentTouchID].x = value;
                    touchPoints[currentTouchID].active = touchPoints[currentTouchID].active && value != 4094;
                }
                break;
            case 0x10030: // Y coordinate
                if (currentTouchID >= 0 && currentTouchID < maxTouches) {
                    touchPoints[currentTouchID].y = value;
                    touchPoints[currentTouchID].active = touchPoints[currentTouchID].active && value != 4094;
                }
                break;
        }
    }

    // Method called at the end of an input report
    virtual void hid_input_end() override {
        for (int i = 0; i < maxTouches; i++) {
            if (touchPoints[i].active){
                #ifdef USESERIAL
                // Print touch point data to Serial if USESERIAL is defined
                Serial.print(" F:");
                Serial.print(i);
                Serial.print(" X:");
                Serial.print(touchPoints[i].x);
                Serial.print(" Y:");
                Serial.print(touchPoints[i].y);
                Serial.print(' ');
                #endif
                #ifdef USETOUCHHID
                // Report the touch point to the TouchscreenUSB if USETOUCHHID is defined
                TouchscreenUSB.press(i, map(touchPoints[i].x, 0L, 4096L, 0L, 32767L), map(touchPoints[i].y, 0L, 4096L, 0L, 32767L));
                #endif
            } else {
                touchPoints[i].active = false;
                
                #ifdef USESERIAL
                // Print touch point release to Serial if USESERIAL is defined
                Serial.print(" D:");
                Serial.print(i);
                Serial.print(' ');
                #endif
                
                #ifdef USETOUCHHID
                // Report the touch point release to the TouchscreenUSB if USETOUCHHID is defined
                TouchscreenUSB.release(i);
                #endif
            }
        }
        Serial.println();

        currentTouchID = -1;

        // Reset touch points for the next report
        resetTouchPoints();
    }

    // Method called when the touch panel device is disconnected
    virtual void disconnect_collection(Device_t *dev) override {
        //Serial.println("Touch panel disconnected");
    }

private:
    // Structure to store touch point data
    struct TouchPoint {
        int32_t x = 0;
        int32_t y = 0;
        bool active = false;
    };

    // Maximum number of touch points
    static const int maxTouches = 5;

    // Array to store touch points
    TouchPoint touchPoints[maxTouches];
    int currentTouchID = -1;

    // Method to reset all touch points
    void resetTouchPoints() {
        for (int i = 0; i < maxTouches; i++) {
            touchPoints[i].active = false;
        }
    }
};
