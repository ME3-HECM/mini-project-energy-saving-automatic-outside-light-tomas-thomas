#ifndef _clock_H
#define _clock_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void UpdateClock(int *seconds, int *minutes, int *hours, int *days, int *DoW, int *months, int *years, int *DSTstate, int TestMode);


#endif