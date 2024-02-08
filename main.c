
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
#include "clock.h"

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
    
    clock_init();

    while (1) {
        
        updateTimer(secs);
        
        LEDarray_disp_bin(clock.seconds);
    
    }
    
}       
//        
//        // creating a global clock 
////        clock(days,hours,mins,secs);
////    //########     TESTING MODE     ########//  if not commented out then the clock will run 1 sec = 1 hour
//        hours = secs;                       // rest of the code does not need to be touched if test mode enabled
//        if (secs >= 24 ){
//        secs = 0; 
//        }
////    //######################################// 
////        
//    // couldn't get this to work without the numbers overflowing 
//        // be very careful with overflow values of reaching the cap of 2^16 
//        
//    // timer module to create time 
//        if (secs >= 60 ){
//        secs = 0;
//        mins++;
//        }
// 
//        if  (mins >= 60){   // by setting this greater than or equal to 60 it ensures that we can't accidentally keep increasing seconds without increase hours
//        mins = 0;
//        hours++;
//        }
//        
//        if (hours >= 24){
//        hours = 0;
//        days++; 
//        }
//        
////        get16bitTMR0val();          // calling the clock value - not sure if this is needed
//        LEDarray_disp_bin(hours);   //displays the time 
//    // 
//        
//        
//        if (secs >= 1 && secs <=5){
//            LED_Right = 1;
//        }
//        else{
//            LED_Right = 0;
//        }



// daylight savings - if clock reaches day x then subtract hours by 1 
// dalight savings pt - if clock reachs day y then add hours by 1 


// change the clock at mid day to avoid issues 

// recalibrate the day before daylight savings 


// measure maximum darkness and brightness throughout the day and check it and then change it according to our solar values
// potentiall we could have it so that we know what our error could be and it only changes within that error to avoid silly calibration issues 