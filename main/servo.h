#ifndef SERVO_H
#define SERVO_H

#include <ESP32Servo.h>

void setup_servo();

extern Servo servo;
extern const int servoPin;

#endif