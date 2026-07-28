#include <Arduino.h>

#include "radar.h"
#include "global.h"

bool radarPresence;

void update_radar(){

  if (false){
      if(radarPresence){
          arm();
      }else{
          disarm();
      }
  }

}