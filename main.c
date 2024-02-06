
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

#define LED_Right LATHbits.LATH3

void main(void) 
{
    unsigned int TestMode = 3600; // scales the second value 1 for normal operation : 3600 for 1 real time second to be 1 hour of the clock
    
    LEDarray_init();        //setting up the LED array 
    Timer0_init();          //setting up the timer
    Interrupts_init();
    
    // left LED board RD7
    TRISDbits.TRISD7 = 0;   // setting the input/output to output
    LATDbits.LATD7 = 0;
    
    TRISHbits.TRISH3 = 0;
    LATHbits.LATH3 = 0;
    
    
    unsigned int mins = 0 ; 
    unsigned int hours = 0; 
    unsigned int days = 0 ;
// #####################

    
    while (1) {
    // creating a global clock 
    
    // couldn't get this to work without the numbers overflowing 
        // be very careful with overflow values of reaching the cap of 2^16 
        
   
//    
//    // timer module to create time 
    if (secs > 60 && ){
        mins = mins + 1;
    }
//    if  (mins >= 60){
//        hours++;
//        mins = 0;
//    }
//    if (hours >= 24){
//        days++;
//        hours = 0;
//    }
    
    get16bitTMR0val();
    LEDarray_disp_bin(mins);   //displays the time 
//    LEDarray_disp_bin(get16bitTMR0val());// gets the 16bit timer value in that instant 
    
    
    if (hours >= 1 && hours <=5){
        LED_Right = 1;
    }
    else{
        LED_Right = 0;
    }
    
    }
}


// daylight savings - if clock reaches day x then subtract hours by 1 
// dalight savings pt - if clock reachs day y then add hours by 1 


// change the clock at mid day to avoid issues 

// recalibrate the day before daylight savings 