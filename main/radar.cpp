#include <Arduino.h>

#include "radar.h"
#include "global.h"

bool radarPresence;
bool toggleRadar = false;

void update_radar(){

  if (toggleRadar){
      if(radarPresence){
          arm(calibration);
      }else{
          disarm();
      }
  }

}