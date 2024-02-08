#ifndef _clock_H
#define _clock_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void clock_init(void);
//void updateTimer(SecondInput);
void UpdateClock(int *seconds, int *minutes, int *hours, int *days, int *months, int *years);


#endif