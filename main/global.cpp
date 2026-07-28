#include "global.h"
#include "servo.h"

bool armed = false;

bool timedRunning;
bool timedArmed;

unsigned long startTime;
unsigned long beforeTime;
unsigned long armedTime;

void arm() {
    servo.write(90);
    armed = true;
}
void disarm() {
    servo.write(0);
    armed = false;
}
void timed(int b, int w) {
    delay(b);
    arm();
    delay(w);
    disarm();
}