#ifndef GLOBAL_H
#define GLOBAL_H

extern bool armed;

extern bool timedRunning;
extern bool timedArmed;

extern unsigned long startTime;
extern unsigned long beforeTime;
extern unsigned long armedTime;
extern int calibration[20];

bool arm(int calibration[20]);
void disarm();
void timed(int b, int w);
void cal(int calibration[20]);
void setup_global();



#endif