
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

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


// NEED TO RECALIBRATE THE CLOCK TO OBTAIN A HIGHER ACCRUACY 
// current testing is 24 seconds according to the board is 23.98 secs IRL

void main(void) 
{
// intialise all functions 
    LEDarray_init();        //setting up the LED array 
    Timer0_init();          //setting up the timer
    Interrupts_init();      //setting up the interrupts
    
    LCD_Init(); 
    ADC_init();
    
    char buffer[18];    //experimental found 18 to be the limit on the LCD screen

// setting up the LEDS on the board with more helpful names
    #define LED_Left LATDbits.LATD7
    TRISDbits.TRISD7 = 0;   // setting up as an output
    LATDbits.LATD7 = 0;     //turning it off
    
    #define LED_Right LATHbits.LATH3
    TRISHbits.TRISH3 = 0;   // setting up as an output
    LATHbits.LATH3 = 0;     //turning it off

    
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
        
//~~~~~~~~~~~~~~~~~~~       TEST    MODE      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int TestMode = 1;       // 0 if off     1 if on
        
        clock.seconds = GLOBALsecs; 
        if (TestMode == 1){
            clock.seconds = 0; 
            GLOBALsecs = clock.hours;   //the global secs will increment our hours value so set the starting value of global sec to our desired hour 
        }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
//    while (1) {
//        
//        clock.seconds = GLOBALsecs;   //updates the clock.seconds to be in sync with clock time
//        UpdateClock(&GLOBALsecs, &clock.minutes, &clock.hours, &clock.days);  //changes the minutes, hours, days in the clock structure when a sec increases
//        
//        //displays the hour value in binary
//        LEDarray_disp_bin(clock.hours);
//        
//        //alternates on to off for each time it changes day
//        if (clock.days % 2 == 0 ){  //shows when a clock day changes on the LED
//            LED_Left = 1;
//        }
//        else{
//            LED_Left = 0; 
//        }
//        
//        // light turning off or on depending on task brief conditions 
//        if (1){ // if the ADC is bigger than our threshold - if dark enough turn on
//            if (clock.hours >= 1 && clock.hours <=5){   //check that its not energy saving time
//                LED_Right = 0;
//            }
//            else{                                       //must not be energy saving time therefore turn light on
//                LED_Right = 1;
//            }
//        }  
//    }  
//      

    // daylight savings - if clock reaches day x then subtract hours by 1 
    // dalight savings pt - if clock reachs day y then add hours by 1 


    // change the clock at mid day to avoid issues 

    // recalibrate the day before daylight savings 


    // measure maximum darkness and brightness throughout the day and check it and then change it according to our solar values
    // potentially we could have it so that we know what our error could be and it only changes within that error to avoid silly calibration issues 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SUN SYNCHRONISATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
    // setting up a time structure to be used for the clock, and time keeping
    struct month_structure { //set up time structure - we haven't actually called it yet
        int solarMidMinutes;
        int solarMidHours;
        int days;
    };

    //All timings are in GMT (+00), therefore we need to account for Daylight saving time with some of the solar midnight values
    struct month_structure Jan;    //creates Jan, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for January taken from timeanddate.com
        Jan.days = 31;
        Jan.solarMidHours = 0;
        Jan.solarMidMinutes = 9;
    
    struct month_structure Feb;    //creates Feb, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for February taken from timeanddate.com
        Feb.days = 28;
        Feb.solarMidHours = 0;
        Feb.solarMidMinutes = 13;
        
    struct month_structure Mar;    //creates Mar, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for March taken from timeanddate.com
        Mar.days = 31;
        Mar.solarMidHours = 0;
        Mar.solarMidMinutes = 8;
        
    struct month_structure Apr;    //creates Apr, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for April taken from timeanddate.com
        Apr.days = 30;
        Apr.solarMidHours = 0; //this is without daylight saving!!
        Apr.solarMidMinutes = 1;
        
    struct month_structure May;    //creates May, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for May taken from timeanddate.com
        May.days = 31;
        May.solarMidHours = 23; //this is without daylight saving!!
        May.solarMidMinutes = 57;
        
    struct month_structure Jun;    //creates Jun, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for June taken from timeanddate.com
        Jun.days = 30;
        Jun.solarMidHours = 0; //this is without daylight saving!!
        Jun.solarMidMinutes = 1;
        
    struct month_structure Jul;    //creates Jul, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for July taken from timeanddate.com
        Jul.days = 31;
        Jul.solarMidHours = 0; //this is without daylight saving!!
        Jul.solarMidMinutes = 5;
        
    struct month_structure Aug;    //creates Aug, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for August taken from timeanddate.com
        Aug.days = 31;
        Aug.solarMidHours = 0; //this is without daylight saving!!
        Aug.solarMidMinutes = 3;
        
    struct month_structure Sep;    //creates Sep, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for September taken from timeanddate.com
        Sep.days = 30;
        Sep.solarMidHours = 23; //this is without daylight saving!!
        Sep.solarMidMinutes = 55;
        
    struct month_structure Oct;    //creates Oct, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for October taken from timeanddate.com
        Oct.days = 31;
        Oct.solarMidHours = 23; //this is without daylight saving!!
        Oct.solarMidMinutes = 47;
        
    struct month_structure Nov;    //creates Nov, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for November taken from timeanddate.com
        Nov.days = 30;
        Nov.solarMidHours = 23;
        Nov.solarMidMinutes = 46;
        
    struct month_structure Dec;    //creates Dec, which is of the structure month_structure
        // set the number of days in the month and the average solar midnight time for December taken from timeanddate.com
        Dec.days = 31;
        Dec.solarMidHours = 23;
        Dec.solarMidMinutes = 56;
        
    
//Define 4 arrays: Dawn and Dusk hours and minutes, with a predetermined size of 7 items.
//This allows us to store the times where dawn and dusk are measured by the LDR over a seven day period
//We can then take averages, calculate average solar midnight and then compare this to our known solar midnight for each month
//The difference can then be added(/subtracted) to re-calibrate/synchronise the clock
   
    struct array_structure { //set up time structure - we haven't actually called it yet
        int size;
        int count;
        int hours;
        int minutes;
    };
    
    struct array_structure Dawn;
        Dawn.size = 7;
        Dawn.count = 0;                                 //ensures that Dawn is only counted once a day
        Dawn.hours = (int[]){0, 0, 0, 0, 0, 0, 0};
        Dawn.minutes = (int[]){0, 0, 0, 0, 0, 0, 0};
        
    struct array_structure Dusk;
        Dusk.size = 7;
        Dusk.count = 0;                                 //ensures that Dusk is only counted once a day
        Dusk.hours = (int[]){0, 0, 0, 0, 0, 0, 0};
        Dusk.minutes = (int[]){0, 0, 0, 0, 0, 0, 0};
    
//    int size = 7; //size of the array to hold 7 days worth of dawn and dusk timings
//    int DawnHours[7] = {0,0,0,0,0,0,0};
//    int DawnMinutes[7] = {0,0,0,0,0,0,0};
//    int DuskHours[7] = {0,0,0,0,0,0,0};
//    int DuskMinutes[7] = {0,0,0,0,0,0,0};
    
    //Function to take measured Dawn and Dusk timings and add it to the 7 day moving average list.
    //Each timing is moved down by one and the new timing is added to the end of the array.
    
    ADC_init(); //setting up the ADC
    
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
                
    while (1) {
        
        if (TestMode == 0){clock.seconds = GLOBALsecs;} 
        //changes the minutes, hours, days, etc in the clock structure when a GlobalSecond increases
        UpdateClock(&GLOBALsecs, &clock.minutes, &clock.hours, &clock.days, &clock.DoW, &clock.months, &clock.years, &clock.DSTstate, TestMode);  
        
//        if (clock.months == 3 && clock.days >= 25 && clock.DoW == 7 && clock.hours >=1){
//        //(*DSTstate == 0) && (*months == 3) && (*days >= 25) && (*DoW == 7) && ( *hours >= 1)
//            TRISDbits.TRISD7 = 0;   // setting up as an output
//            LATDbits.LATD7 = 1;     //turning it off
//        }
        //displays the hour value in binary on the LED display
        LEDarray_disp_bin(clock.hours);
        
        
        //setting up the LCD screen to display our values
        LCD_setline(1);
        //displays the hours, minutes, seconds, and day of the week in the clock
        sprintf(buffer, "Time:%02d:%02d:%02d D%01d",clock.hours, clock.minutes, clock.seconds, clock.DoW); //Sets buffer string to take the first int part value and the first 2 values in the frac part variable
        LCD_sendstring(buffer); //Prints string buf  in LCD display
        LCD_setline(2);
        //displays the days, months, and years of the clock
        sprintf(buffer, "Date:%02d/%02d/%04d",clock.days, clock.months, clock.years);
        LCD_sendstring(buffer); //Prints buf value in LCD display
       
//// #####################################
//        //debugging tools 
//        //alternates on to off for each time it changes day
//        if (clock.months == 3 ){  //shows when a clock day changes on the LED
//            LED_Left = 1;
//        }
//        else{
//            LED_Left = 0; 
//        }
////        
//        // light turning off or on depending on task brief conditions 
//        if (1){ // if the ADC is bigger than our threshold - if dark enough turn on
//            if (clock.hours >= 1 && clock.hours <=5){   //check that its not energy saving time
//                LED_Right = 0;
//            }
//            else{                                       //must not be energy saving time therefore turn light on
//                LED_Right = 1;
//            }
//        } 
//        
//       
//       
       
        //alternates on to off for each time it changes day
//        if (clock.days % 2 == 0 ){  //shows when a clock day changes on the LED
//            LED_Left = 1;
//        }
//        else{
//            LED_Left = 0; 
//        }
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
        //check current light level from LDR
        unsigned int curval = ADC_getval();
        
        // light turning off or on depending on task brief conditions 
        if (curval < light_threshold){  // if the ADC is bigger than our threshold - if dark enough turn on
            
            
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
        
        if (curval > light_threshold){ //if light enough, turn LED off
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
            
            LED_Left = 1;
            previousClockDays = clock.days;
        
            if (daycount == 7) {
                
                //function here to average all the dawn/dusk times and compare with known value
                
                
                
            
                
                
                
                LED_Left = 1;
                LED_Right = 1;
                __delay_ms(500);
                
                
                //correction added to time
                
                daycount = 0;
            }
        }
    }  
}
        