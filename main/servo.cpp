#include "servo.h"

Servo servo;
const int servoPin = 18;

void setup_servo(){

  servo.attach(servoPin);

}