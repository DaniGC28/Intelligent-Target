#ifndef GLOBAL_H
#define GLOBAL_H

extern bool armed;

extern bool timedRunning;
extern bool timedArmed;

extern unsigned long startTime;
extern unsigned long beforeTime;
extern unsigned long armedTime;

void arm();
void disarm();
void timed(int b, int w);

#endif