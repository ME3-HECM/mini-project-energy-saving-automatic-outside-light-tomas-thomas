
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


// NEED TO RECALIBRATE THE CLOCK TO OBTAIN A HIGHER ACCRUACY 
// current testing is 24 seconds according to the board is 23.98 secs IRL

void main(void) 
{
// intialises all functions 
    LEDarray_init();        //setting up the LED array 
    Timer0_init();          //setting up the timer
    Interrupts_init();      //setting up the interrupts

// setting up the LEDS on the board with more helpful names
    #define LED_Left LATDbits.LATD7
    TRISDbits.TRISD7 = 0;   // setting up as an output
    LATDbits.LATD7 = 0;     //turning it off
    
    #define LED_Right LATHbits.LATH3
    TRISHbits.TRISH3 = 0;   // setting up as an output
    LATHbits.LATH3 = 0;     //turning it off

// setting up a time structure to be used for the clock, and time keeping
     struct time_structure { //set up time structure - we haven't actually called it yet
        int seconds;
        int minutes;
        int hours;
        int days;
//        int months;
//        char month;   // potentially use this to track the month and output onto LED display
    };

    struct time_structure clock;    //creates clock, which is of the structure time_structure
        // set the initial starting time when the sensor is set up
        secs = 50;
        clock.minutes = 59;
        clock.hours = 12;
        clock.days = 1;
//        clock.months = 0;
   
        // last Sunday in March and October.
    struct time_structure DSTon;
        DSTon.minutes = 0;
        DSTon.hours = 0;
        DSTon.days = 0;
    
    
//~~~~~~~~~~~~~~~~~~~       TEST    MODE      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
    //1. turn on the function below:
        secs = clock.hours;       //turn on
    
    //2. AND go into clock.c and turn on testing mode
        
    //this will equate seconds in IRL to hours in terms of the display.
        
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    while (1) {
        
        clock.seconds = secs;   //updates the clock.seconds to be in sync with clock time
        UpdateClock(&secs, &clock.minutes, &clock.hours, &clock.days);  //changes the minutes, hours, days in the clock structure when a sec increases
        
        LEDarray_disp_bin(clock.hours);
        
        //alternates on to off for each time it changes day
        if (clock.days % 2 == 0 ){  //shows when a clock day changes on the LED
            LED_Left = 1;
        }
        else{
            LED_Left = 0; 
        }
        
        // light turning off or on depending on task brief conditions 
        if (1){ // if the ADC is bigger than our threshold - if dark enough turn on
            if (clock.hours >= 1 && clock.hours <=5){   //check that its not energy saving time
                LED_Right = 0;
            }
            else{                                       //must not be energy saving time therefore turn light on
                LED_Right = 1;
            }
        }  
    }  
}       


// daylight savings - if clock reaches day x then subtract hours by 1 
// dalight savings pt - if clock reachs day y then add hours by 1 


// change the clock at mid day to avoid issues 

// recalibrate the day before daylight savings 


// measure maximum darkness and brightness throughout the day and check it and then change it according to our solar values
// potentiall we could have it so that we know what our error could be and it only changes within that error to avoid silly calibration issues 