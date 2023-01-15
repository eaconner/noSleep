#include <Arduino.h>
#include <Mouse.h>

void setup() {
    Mouse.begin();
    randomSeed(analogRead(0));
}

void loop() {
    delay(random(20000,55000));
    Mouse.move(1, 0, 0);
    Mouse.move(-1, 0, 0);
}
