#ifndef _clock_H
#define _clock_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void updateTimer(SecondInput);
void clock_init(void);

#endif