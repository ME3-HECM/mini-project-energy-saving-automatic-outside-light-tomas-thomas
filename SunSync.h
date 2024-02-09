#ifndef _SunSync_H
#define _SunSync_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void SunSynnInit();
void LightDetection(int ADC_val, int hours);
int DuskAndDawnCollect(int ADC_val, int months, int days, int hours, int mins, int DSTstate, *DawnDetected, *DuskDetected, *DawnStartMins,*DawnStartHours, *DuskStartMins, *DuskStartHours, KnownSolarMins, KnownSolarHours);    // on the input CSM[clock.months - 1]

#endif
