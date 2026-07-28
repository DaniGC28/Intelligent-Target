#include "global.h"
#include "piezo.h"
#include "servo.h"
#include "web.h"
#include "ble.h"
#include "radar.h"



void setup() {
  Serial.begin(115200);

  setup_web();
  setup_servo();
  setup_ble();

  disarm();
  
}


void loop() {

    update_web();

    if (timedRunning) {

        unsigned long elapsed = millis() - startTime;

        if (!timedArmed && elapsed >= beforeTime) {
            arm();
            timedArmed = true;
        }

        if (timedArmed && elapsed >= beforeTime + armedTime) {
            disarm();
            timedRunning = false;
        }
    }

    update_piezo();
    update_ble();
    update_radar();
    
}