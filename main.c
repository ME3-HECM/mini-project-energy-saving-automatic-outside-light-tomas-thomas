
// ###############################################################pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

// ##############################################################

// include all .h files here
#include <xc.h>

#include "LEDarray.h"
#include "timers.h"
#include "interrupts.h"
#include "seconds.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) 
{
    unsigned int TestMode = 3600; // scales the second value 1 for normal operation : 3600 for 1 real time second to be 1 hour of the clock
    LEDarray_init();        //setting up the LED array 
    Timer0_init();          //setting up the timer
    Interrupts_init();
    
    
    
    while (1) {
    // creating a global clock 
    
    unsigned int mins = secs*(TestMode)/(60); 
    unsigned int hours = mins/60;
    unsigned int days = hours/24;
//    if (hours>24){
//        secs = 0; // resets the clock to zero when 1 day has passed
//    }
    get16bitTMR0val();
    LEDarray_disp_bin(hours);   //displays the time 
//    LEDarray_disp_bin(get16bitTMR0val());// gets the 16bit timer value in that instant 
    }
}
