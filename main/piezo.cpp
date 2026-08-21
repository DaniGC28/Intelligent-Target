#include <Arduino.h>
#include "piezo.h"
#include "global.h"

const int piezoPin = 34;
int thresholdPiezo = 100;
bool togglePiezo = false;

void update_piezo(){
  if (togglePiezo){
    if (analogRead(piezoPin) > thresholdPiezo && armed){
            disarm();
        }
  }
}