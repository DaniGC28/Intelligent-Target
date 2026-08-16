#include "global.h"
#include "servo.h"
#include "piezo.h"

bool armed = false;

bool timedRunning;
bool timedArmed;

unsigned long startTime;
unsigned long beforeTime;
unsigned long armedTime;

int calibration[20];

bool arm(int calibration[20]) {
    servo.write(90);
    int i = 0;
    unsigned long int start = millis();
    while (i < 20){
        if ((millis() - start) >= 50*i){
            if (calibration[i]*1.2 <  analogRead(piezoPin)){
                disarm();
                return false;
            }
            i += 1;
        }
    }
    armed = true;
    return true;
}
void disarm() {
    servo.write(0);
    armed = false;
}
void timed(int b, int w) {
    delay(b);
    arm(calibration);
    delay(w);
    disarm();
}
void cal(int calibration[20]) {
    unsigned long int start = millis();
    int i = 0;
    servo.write(90);
    while (i < 20){
        if ((millis() - start) >= 50*i){
            calibration[i] = analogRead(piezoPin);
            i += 1;
        }
    }
    disarm();
}

void setup_global(){
    cal(calibration);
}