#include <Arduino.h>
#include "piezo.h"
#include "global.h"

const int piezoPin = 34;
int thresholdPiezo = 100;

void update_piezo(){

  if (analogRead(piezoPin) > thresholdPiezo && armed){
        disarm();
    }

}