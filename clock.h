#ifndef _clock_H
#define _clock_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void clock(int d, int h, int m, int s);
void clock_init(void);

#endif