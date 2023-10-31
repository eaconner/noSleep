/**
 * @file main.cpp
 * @brief Revised version of noSleep dongle that adds enable/disable mouse movement switch with status LED and a Teams mute button with status LED.
 * 
 * @author Eric Conner (Eric@EricConner.net)
 * @version 2.0
 * @copyright Copyright (c) 2022
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>
#include <Mouse.h>
#include <Keyboard.h>

// CONSTANTS
const int ENABLE_SWITCH = 2;
const int ENABLE_LED = 4;
const int MUTE_BUTTON = 6;
const int MUTE_LED = 8;

// VARIABLES
unsigned long lastMoveTime = 0;
unsigned long randomMouseDelay = 0;
boolean buttonState;
boolean lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
boolean muteLEDState = LOW;

/**
 * @brief Check to see if the mouse should be moved randomly.
 * 
 * @return boolean 
 */
boolean shouldMoveMouse() {
    if (digitalRead(ENABLE_SWITCH) == LOW) {
        digitalWrite(ENABLE_LED, HIGH);

        if((millis() - lastMoveTime) > randomMouseDelay) {
            lastMoveTime = millis();
            randomMouseDelay = random(20000,55000);
            return true;
        }
    }

    digitalWrite(ENABLE_LED, LOW);
    return false;
}

/**
 * @brief Move the mouse by +1px then -1px.
 * 
 */
void moveMouse() {
    Mouse.move(1, 0, 0);
    Mouse.move(-1, 0, 0);
}

/**
 * @brief Check if the mute button was pressed.
 * 
 * @return boolean 
 */
boolean shouldMuteTeams() {
    // Read the value of the Mute Button and store it as reading.
    int reading = digitalRead(MUTE_BUTTON);

    // Check if reading is different from the lastButtonState
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
        lastButtonState = reading;
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading == LOW) {
            buttonState = reading;
            return true;
        }
    }

    return false;
}

/**
 * @brief Send keyboard commands to mute Teams Call.
 * 
 */
void sendMuteCommand() {
    // Send keyboard shortcut Ctrl+Shift+M to Mute Teams Call
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('m');
    delay(100);
    Keyboard.releaseAll();

    // Toggle LED
    muteLEDState = !muteLEDState;
    digitalWrite(MUTE_LED, muteLEDState);
    delay(1000);
}

/**
 * @brief Setup the Mouse, Keyboard, I/O, and random seed.
 * 
 */
void setup() {
    Mouse.begin();
    Keyboard.begin();

    pinMode(ENABLE_SWITCH, INPUT_PULLUP);
    pinMode(ENABLE_LED, OUTPUT);
    pinMode(MUTE_BUTTON, INPUT_PULLUP);
    pinMode(MUTE_LED, OUTPUT);

    digitalWrite(ENABLE_LED, LOW);
    digitalWrite(MUTE_LED, muteLEDState);

    randomSeed(analogRead(0));
    randomMouseDelay = random(20000,55000);
}

/**
 * @brief Continuiosly check if the mouse need to be moved and if the mute button was pressed.
 * 
 */
void loop() {
    if (shouldMoveMouse()) {
        moveMouse();
    }
    
    if (shouldMuteTeams()) {
        sendMuteCommand();
    }
}
