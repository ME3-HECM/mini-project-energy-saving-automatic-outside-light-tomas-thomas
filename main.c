#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

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

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) 
{
// intialision of all functions 
    LEDarray_init();        //setting up the LED array 
    Timer0_init();          //setting up the timer
    Interrupts_init();      //setting up the interrupts
    LCD_Init();             //LCD display intialisation
    ADC_init();             //Analogue to digital conversion intialisation
    
    char buffer[18];    //provides a string for the LCD screen to write to and then display // experimental found 18 to be the limit on the LCD screen

// setting up the LEDS on the board with more helpful names
    #define LED_Left LATDbits.LATD7
    TRISDbits.TRISD7 = 0;   // setting up as an output
    LATDbits.LATD7 = 0;     //turning it off
    
    #define LED_Right LATHbits.LATH3
    TRISHbits.TRISH3 = 0;   // setting up as an output
    LATHbits.LATH3 = 0;     //turning it off

    
// setting up a time structure to be used for the clock, and time keeping
     struct time_structure { //set up time structure - we haven't actually called it yet
        unsigned int seconds; unsigned int minutes; unsigned int hours; unsigned int days; unsigned int DoW; unsigned int months; unsigned int years; unsigned int DSTstate;
    };
   //~~~~~~~~~~~~~~~~~~~         TEST     MODE        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
    int TestMode = 1;       // 0 if off     1 if on
        
    
    
   //~~~~~~~~~~~~~~~~~~~       Time Data Input         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    struct time_structure clock;    //creates clock, which is of the structure time_structure
        // set the initial starting time when the sensor is set up
        GLOBALsecs = 55;
        clock.minutes = 59;
        clock.hours = 22;
        clock.days = 28;
        clock.DoW = 5;              //1-Monday 2-Tuesday 3-Wednesday 4-Thursday 5-Friday 6-Saturday 7-Sunday
        clock.months = 2;
        clock.years = 2024;
        clock.DSTstate = 0;         // is your input date in daylight savings time March-October (1) or not October-March (0)
        
   //DAYLIGHT SAVINGS TURN OFF - OCTOBER 
//        clock.minutes = 59;
//        clock.hours = 0;
//        clock.days = 30;
//        clock.DoW = 7;              
//        clock.months = 10;
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
            
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SUN SYNCHRONISATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
    // setting up a time structure to be used for the clock, and time keeping
    struct month_structure { //set up time structure - we haven't actually called it yet
        int solarMidMinutes[12];
        int solarMidHours[12];
        int days[12];
    };
    
    struct month_structure SolarPerMonth;
    SolarPerMonth.days[0] = 31;
    SolarPerMonth.days[1] = 28;
    SolarPerMonth.days[2] = 31;
    SolarPerMonth.days[3] = 30;
    SolarPerMonth.days[4] = 31;
    SolarPerMonth.days[5] = 30;
    SolarPerMonth.days[6] = 31;
    SolarPerMonth.days[7] = 31;
    SolarPerMonth.days[8] = 30;
    SolarPerMonth.days[9] = 31;
    SolarPerMonth.days[10] = 30;
    SolarPerMonth.days[11] = 31;

    SolarPerMonth.solarMidHours[0] = 0;
    SolarPerMonth.solarMidHours[1] = 0;
    SolarPerMonth.solarMidHours[2] = 0;
    SolarPerMonth.solarMidHours[3] = 0;
    SolarPerMonth.solarMidHours[4] = 23;
    SolarPerMonth.solarMidHours[5] = 0;
    SolarPerMonth.solarMidHours[6] = 0;
    SolarPerMonth.solarMidHours[7] = 0;
    SolarPerMonth.solarMidHours[8] = 23;
    SolarPerMonth.solarMidHours[9] = 23;
    SolarPerMonth.solarMidHours[10] = 23;
    SolarPerMonth.solarMidHours[11] = 23;

    SolarPerMonth.solarMidMinutes[0] = 9;
    SolarPerMonth.solarMidMinutes[1] = 13;
    SolarPerMonth.solarMidMinutes[2] = 8;
    SolarPerMonth.solarMidMinutes[3] = 1;
    SolarPerMonth.solarMidMinutes[4] = 57;
    SolarPerMonth.solarMidMinutes[5] = 1;
    SolarPerMonth.solarMidMinutes[6] = 5;
    SolarPerMonth.solarMidMinutes[7] = 3;
    SolarPerMonth.solarMidMinutes[8] = 55;
    SolarPerMonth.solarMidMinutes[9] = 47;
    SolarPerMonth.solarMidMinutes[10] = 46;
    SolarPerMonth.solarMidMinutes[11] = 56;

    //All timings are in GMT (+00), therefore we need to account for Daylight saving time with some of the solar midnight values
    
//Define 4 arrays: Dawn and Dusk hours and minutes, with a predetermined size of 7 items.
//This allows us to store the times where dawn and dusk are measured by the LDR over a seven day period
//We can then take averages, calculate average solar midnight and then compare this to our known solar midnight for each month
//The difference can then be added(/subtracted) to re-calibrate/synchronise the clock
   
    struct array_structure { //set up time structure - we haven't actually called it yet
        int size;
        int count;
        int hours[7];
        int minutes[7];
    };
    
    struct array_structure Dawn;
        Dawn.size = 7;
        Dawn.count = 0;                                 //ensures that Dawn is only counted once a day
        
        Dawn.hours[0] = 0;
        Dawn.hours[1] = 0;
        Dawn.hours[2] = 0;
        Dawn.hours[3] = 0;
        Dawn.hours[4] = 0;
        Dawn.hours[5] = 0;
        Dawn.hours[6] = 0;

        Dawn.minutes[0] = 0;
        Dawn.minutes[1] = 0;
        Dawn.minutes[2] = 0;
        Dawn.minutes[3] = 0;
        Dawn.minutes[4] = 0;
        Dawn.minutes[5] = 0;
        Dawn.minutes[6] = 0;
        
    struct array_structure Dusk;
        Dusk.size = 7;
        Dusk.count = 0;                                 //ensures that Dusk is only counted once a day

        Dusk.hours[0] = 0;
        Dusk.hours[1] = 0;
        Dusk.hours[2] = 0;
        Dusk.hours[3] = 0;
        Dusk.hours[4] = 0;
        Dusk.hours[5] = 0;
        Dusk.hours[6] = 0;

        Dusk.minutes[0] = 0;
        Dusk.minutes[1] = 0;
        Dusk.minutes[2] = 0;
        Dusk.minutes[3] = 0;
        Dusk.minutes[4] = 0;
        Dusk.minutes[5] = 0;
        Dusk.minutes[6] = 0;



//    int size = 7; //size of the array to hold 7 days worth of dawn and dusk timings
//    int DawnHours[7] = {0,0,0,0,0,0,0};
//    int DawnMinutes[7] = {0,0,0,0,0,0,0};
//    int DuskHours[7] = {0,0,0,0,0,0,0};
//    int DuskMinutes[7] = {0,0,0,0,0,0,0};
    
    //Function to take measured Dawn and Dusk timings and add it to the 7 day moving average list.
    //Each timing is moved down by one and the new timing is added to the end of the array.
    
    
    //Defining light levels for turning LED on in dark and off in light
    unsigned int light_threshold = 70;     // CalibrationNumber any number from 0 to 255  - Used as transition threshold light level at dawn/dusk.
                                            //Only one light level is needed - represents both Dawn and Dusk - this is the transition from light to dark and vice versa.
    
    unsigned int daycount = 0; //keep track of the number of days since the last Sun synchronisation. 
                               //this will go up to 7 where it will calculate the average solar midnight and then compare with known value
                               //for re-calibration.
    unsigned int previousClockDays = clock.days; //To store the current/previous day to indicate when a day has passed.
    
    
     //This is to display bits to calibrate LDR for Dawn and Dusk
//      unsigned int calibration_level = ADC_getval();
//      LEDarray_disp_bin(calibration_level);
          
    clock.seconds = GLOBALsecs; 
        if (TestMode == 1){
            clock.seconds = 0; 
            GLOBALsecs = clock.hours;   //the global secs will increment our hours value so set the starting value of global sec to our desired hour 
        }
    
    while (1) {
       
        //GLOBAL CLOCK
        if (TestMode == 0){clock.seconds = GLOBALsecs;} 
        UpdateClock(&GLOBALsecs, &clock.minutes, &clock.hours, &clock.days, &clock.DoW, &clock.months, &clock.years, &clock.DSTstate, TestMode);    //changes the minutes, hours, days, etc in the clock structure when a GlobalSecond increases
        
        //LED display in binary
        LEDarray_disp_bin(clock.hours);     //displays the hour value in binary on the LED display
        
        //setting up the LCD screen to display our values
        LCD_setline(1);
        sprintf(buffer, "Time:%02d:%02d:%02d D%01d",clock.hours, clock.minutes, clock.seconds, clock.DoW); //Sets buffer string to take the first int part value and the first 2 values in the frac part variable
        LCD_sendstring(buffer); //Prints string buf  in LCD display - //displays the hours, minutes, seconds, and day of the week in the clock
        LCD_setline(2);
        sprintf(buffer, "Date:%02d/%02d/%04d",clock.days, clock.months, clock.years);
        LCD_sendstring(buffer); //Prints buf value in LCD  -  displays the days, months, and years of the clock

        // light turning off or on depending on task brief conditions 
        if (ADC_getval() < light_threshold){  // if the ADC is bigger than our threshold - if dark enough turn on
            if ((clock.hours >= 1 && clock.hours < 5) || (clock.hours >= 8 && clock.hours < 15)) {   //check that its not energy saving time or that nothing external is blocking the LDR during daylight hours. 
                LED_Right = 0;                                  //this ensures that if external factors such as birds block the LDR,
            }                                                    //the light will not go on during the day. 8am-3pm chosen, as dawn/dusk can be as late/early as 8-3 respectively at winter solstice.
            
            else {                  //If light levels have lowered further than the threshold at dusk, turn the LED on.
                LED_Right = 1;      //must not be energy saving time therefore turn light on
                if ((Dusk.count = 0)&&(clock.hours >=15 && clock.hours < 8)) {
                    ArrayAppend(Dusk.hours, Dusk.size, clock.hours);
                    ArrayAppend(Dusk.minutes, Dusk.size,  clock.minutes);
                    Dusk.count = 1;
                }
            }
        }  
        
        
        if (ADC_getval() > light_threshold){ //if light enough, turn LED off
            LED_Right = 0;
            if ((Dawn.count = 0)&&(clock.hours >=4 && clock.hours < 8)) { //Dawn only occurs between 4am and 8am
                ArrayAppend(Dawn.hours, Dawn.size, clock.hours);
                ArrayAppend(Dawn.minutes, Dawn.size,  clock.minutes);
                Dawn.count = 1;
            }
        }
        
        if (clock.days > previousClockDays) {
            Dawn.count = 0;
            Dusk.count = 0;
            daycount++;
            
            previousClockDays = clock.days;
        
            if (daycount == 7) {
                
                //function here to average all the dawn/dusk times and compare with known value
                
                //solar midnight occurs naturally between 23:00 and 01:00 GMT 
                
                int operation = 0;
                
                int hours_temp = SolarPerMonth.solarMidHours[(clock.months - 1)]; //average solar midnight hour time for the specific month
                int minutes_temp = SolarPerMonth.solarMidMinutes[(clock.months - 1)]; //average solar midnight minutes time for the specific month
                
                if (clock.DSTstate = 1) {hours_temp = hours_temp + 1;} //account for daylight saving time in the known solar midnight values which are GMT
                if (hours_temp = 0 || hours_temp == 1) {hours_temp = hours_temp + 24;} //if solar midnight is past midnight, reference this as 24 in the hours rather than 00
                
                
                int knownSolarMidnight = hours_temp*60 + minutes_temp;
                
                struct array_structure SolarMidnight;
                    SolarMidnight.minutes[0] = 0;
                    SolarMidnight.minutes[1] = 0;
                    SolarMidnight.minutes[2] = 0;
                    SolarMidnight.minutes[3] = 0;
                    SolarMidnight.minutes[4] = 0;
                    SolarMidnight.minutes[5] = 0;
                    SolarMidnight.minutes[6] = 0;
                                                    //array defining the total minutes for solar midnight

                for (int i = 0; i <= Dawn.size-1; i++) {
                    operation = (Dawn.hours[i] * 60 + Dawn.minutes[i]) + (Dusk.hours[i]*60 + Dusk.minutes[i]); //sum their total minutes
                    operation = operation * 0.5 + 12*60; //find the average of the sum of their minutes and then add 12 hours to find solar midnight in minutes
                    ArrayAppend(SolarMidnight.minutes, 7, operation); //add the solar midnight value to the array
                }
                int avgSolarMidnight = ArrayAverage(SolarMidnight.minutes, 7); //calculate average solar midnight from measurements over past week
                int minute_diff = knownSolarMidnight - avgSolarMidnight; //compare with known solar midnight
                
                
                clock.minutes = clock.minutes + minute_diff;
                
                //correction added to time
                
                daycount = 0;
            }
        }
    }  
}
        