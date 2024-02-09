
// ###############################################################pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

// ##############################################################

// include all .h files here
#include <xc.h>
#include <stdio.h>

#include "LEDarray.h"
#include "timers.h"
#include "interrupts.h"
#include "seconds.h"
#include "clock.h"
#include "LCD.h"
#include "ADC.h"
#include "SunSync.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


// NEED TO RECALIBRATE THE CLOCK TO OBTAIN A HIGHER ACCRUACY 
// current testing is 24 seconds according to the board is 23.98 secs IRL

void main(void) 
{
// intialise all functions 
    LEDarray_init();        //setting up the LED array 
    Timer0_init();          //setting up the timer
    Interrupts_init();      //setting up the interrupts
    SunSynnInit();          //sets up the sun synchronicity
    LCD_Init(); 
    ADC_init();
    
    char buffer[18];    //experimental found 18 to be the limit on the LCD screen
    
    struct month_structure { //set up time structure - we haven't actually called it yet
        int MidMinutes[12];
        int MidHours[12];
        int days[12];
};

struct month_structure Solar = {
    {9, 13, 8, 1, 57, 1, 5, 3, 55, 47, 46, 56},       // Solar mid-minutes
    {0, 0, 0, 0, 23, 0, 0, 0, 23, 23, 23, 23},        // Solar mid-hours
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} // Days in each month
};
        int DawnStartMins;
        int DawnStartHours = 0;
        int DuskStartMins;
        int DuskStartHours = 0;
        int DawnDetected=0;
        int DuskDetected=0;
        
        unsigned int delta;    
    
    
// setting up a time structure to be used for the clock, and time keeping
     struct time_structure { //set up time structure - we haven't actually called it yet
        unsigned int seconds;
        unsigned int minutes;
        unsigned int hours;
        unsigned int days;
        unsigned int DoW;
        unsigned int months;
        unsigned int years;
        unsigned int DSTstate;
    };
   
    struct time_structure clock;    //creates clock, which is of the structure time_structure
        // set the initial starting time when the sensor is set up
        GLOBALsecs = 55;
        clock.minutes = 59;
        clock.hours = 20;
        clock.days = 31;
        clock.DoW = 1;              //1-Monday 2-Tuesday 3-Wednesday 4-Thursday 5-Friday 6-Saturday 7-Sunday
        clock.months = 12;
        clock.years = 2024;
        clock.DSTstate = 0;         // is your input date in daylight savings time March-October (1) or not October-March (0)
        
   //DAYLIGHT SAVINGS TURN OFF - OCTOBER 
//        clock.minutes = 59;
//        clock.hours = 1;
//        clock.days = 30;
//        clock.DoW = 7;              
//        clock.months = 01;
//        clock.years = 2022;
//        clock.DSTstate = 1;
        
   //DAYLIGHT SAVINGS TURN ON - March
//        clock.minutes = 59;
//        clock.hours = 0;
//        clock.days = 31;
//        clock.DoW = 7;              
//        clock.months = 3;
//        clock.years = 2024;
//        clock.DSTstate = 0;
   
//~~~~~~~~~~~~~~~~~~~       TEST    MODE      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int TestMode = 1;       // 0 if off     1 if on
        
      clock.seconds = GLOBALsecs;
        if (TestMode == 1){
            clock.seconds = 0; 
            GLOBALsecs = clock.hours;   //the global secs will increment our hours value so set the starting value of global sec to our desired hour 
        }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       
        
    while (1) {
        
        if (TestMode == 0){clock.seconds = GLOBALsecs;} 
        //changes the minutes, hours, days, etc in the clock structure when a GlobalSecond increases
        UpdateClock(&GLOBALsecs, &clock.minutes, &clock.hours, &clock.days, &clock.DoW, &clock.months, &clock.years, &clock.DSTstate, TestMode);  
        
        //displays the hour value in binary on the LED display
        LEDarray_disp_bin(clock.hours);
        
        LightDetection(ADC_getval(), clock.hours);
        
        if(clock.hours >= 16 && (ADC_getval()<=70) && DuskDetected == 0){
            clock.hours;
            clock.minutes;
            
        }
        delta = DuskAndDawnCollect(ADC_getval(), clock.months, clock.days, clock.hours, clock.minutes, clock.DSTstate, &DawnDetected, &DuskDetected, &DawnStartMins,&DawnStartHours, &DuskStartMins, &DuskStartHours, Solar.MidMinutes[clock.months - 1], Solar.MidHours[clock.months - 1]);
//        clock.minutes = clock.minutes - delta;
        
        //setting up the LCD screen to display our values
        LCD_setline(1);
        //displays the hours, minutes, seconds, and day of the week in the clock
        sprintf(buffer, "Time:%02d:%02d:%02d D%01d",clock.hours, clock.minutes, clock.seconds, clock.DoW); //Sets buffer string to take the first int part value and the first 2 values in the frac part variable
        LCD_sendstring(buffer); //Prints string buffer  in LCD display
        LCD_setline(2);
        //displays the days, months, and years of the clock
        sprintf(buffer, "Date:%02d/%02d/%04d",clock.days, clock.months, clock.years);
        LCD_sendstring(buffer); //Prints buffer value in LCD display
       
    }  
}       
