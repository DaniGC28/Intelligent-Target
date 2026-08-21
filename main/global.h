#ifndef GLOBAL_H
#define GLOBAL_H

extern bool armed;

extern bool timedRunning;
extern bool timedArmed;
extern bool timedSide;

extern unsigned long startTime;
extern unsigned long beforeTime;
extern unsigned long armedTime;

extern int calibration[20];
extern int lPos;
extern int rPos;

bool arm(int calibration[20]);
void disarm();
void cal(int calibration[20]);

void setup_global();
void update_global();


#endif