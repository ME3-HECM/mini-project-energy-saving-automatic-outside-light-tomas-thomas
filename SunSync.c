#include <xc.h>
#include "SunSync.h"
#include "seconds.h"

void SunSynnInit(){
    struct month_structure { //set up time structure - we haven't actually called it yet
        int days[12];
        int MidHours[12];
        int MidMinutes[12]; 
    };
    struct month_structure solar = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, // Days in each month
    {0, 0, 0, 0, 23, 0, 0, 0, 23, 23, 23, 23},        // Solar mid-hours
    {9, 13, 8, 1, 57, 1, 5, 3, 55, 47, 46, 56}       // Solar mid-minutes
    };
   
    TRISHbits.TRISH3 = 0;   // setting up as an output
    LATHbits.LATH3 = 0; 
} 

void LightDetection(int light, int hours){
    if (light <= 70 && ((hours < 1) || (hours>=5)) ){   //turn on if there is less than 70 light and not betweeen 1 and 5am.
        LATHbits.LATH3 = 1;
    }
    else{
       LATHbits.LATH3 = 0; 
    } 
}

int DuskAndDawnCollect(int ADC_val, int months, int days, int hours, int mins, int DSTstate, *DawnDetected, *DuskDetected, *DawnStartMins,*DawnStartHours, *DuskStartMins, *DuskStartHours, KnownSolarMins, KnownSolarHours){    // on the input CSM[clock.months - 1]
    int SolarMidnightCalculated;
    int SolarMidnightConstant;
    int delta;
    
    if (*DawnDetected == 0 && (ADC_val >= 70) && (hours >= 4 && hours < 10)){ //if we've seen light, and we're in the hours we expect to see dawn
        *DawnStartHours = hours;
        *DawnStartMins = mins;
        *DawnDetected = 1;
        //add the ADC_value to the list 
    }
     if (*DuskDetected == 0 && (ADC_val <= 70) && (hours >= 15 && hours < 21)){ //if we've seen light, and we're in the hours we expect to see dawn
        *DuskStartHours = hours;
        *DuskStartMins = mins;
        *DuskDetected = 1;
        //add the ADC_value to the list 
    }
    SolarMidnightCalculated = 12*60 + 0.5*((*DawnStartHours * 60 + *DawnStartMins) + (*DuskStartHours * 60 + *DuskStartMins));
              
    if (DSTstate == 1) {
        KnownSolarHours = KnownSolarHours + 1;
    } //account for daylight saving time in the known solar midnight values which are GMT
    if (KnownSolarHours == 0 || KnownSolarHours == 1) {KnownSolarHours = KnownSolarHours + 24;} //if solar midnight is past midnight, reference this as 24 in the hours rather than 00
        
    SolarMidnightConstant =  (KnownSolarHours * 60 + KnownSolarMins) ; 
    delta = SolarMidnightConstant - SolarMidnightCalculated;
    return(delta);
}

//experimental 7 day average scheme
//void SunFun(){
//    if (ADC_getval() > light_threshold){ //if light enough, turn LED off
//            LED_Right = 0;
//            if ((Dawn.count == 0)&&(clock.hours >=4 && clock.hours < 8)) { //Dawn only occurs between 4am and 8am
//                ArrayAppend(Dawn.hours, Dawn.size, clock.hours);
//                ArrayAppend(Dawn.minutes, Dawn.size,  clock.minutes);
//                Dawn.count = 1;
//            }
//        }
//        
//        if (clock.days > previousClockDays) {
//            Dawn.count = 0;
//            Dusk.count = 0;
//            daycount++;
//            
//            previousClockDays = clock.days;
//        
//            if (daycount == 7) {
//                
//                //function here to average all the dawn/dusk times and compare with known value
//                
//                //solar midnight occurs naturally between 23:00 and 01:00 GMT 
//                
//                int operation = 0;
//                
//                int hours_temp = SolarPerMonth.solarMidHours[(clock.months - 1)]; //average solar midnight hour time for the specific month
//                int minutes_temp = SolarPerMonth.solarMidMinutes[(clock.months - 1)]; //average solar midnight minutes time for the specific month
//                
//              
//                
//                
//                int knownSolarMidnight = hours_temp*60 + minutes_temp;
//                
//                struct array_structure SolarMidnight;
//                    SolarMidnight.minutes[0] = 0;
//                    SolarMidnight.minutes[1] = 0;
//                    SolarMidnight.minutes[2] = 0;
//                    SolarMidnight.minutes[3] = 0;
//                    SolarMidnight.minutes[4] = 0;
//                    SolarMidnight.minutes[5] = 0;
//                    SolarMidnight.minutes[6] = 0;
//                                                    //array defining the total minutes for solar midnight
//
//                for (int i = 0; i <= Dawn.size-1; i++) {
//                    operation = (Dawn.hours[i] * 60 + Dawn.minutes[i]) + (Dusk.hours[i]*60 + Dusk.minutes[i]); //sum their total minutes
//                    operation = operation * 0.5 + 12*60; //find the average of the sum of their minutes and then add 12 hours to find solar midnight in minutes
//                    ArrayAppend(SolarMidnight.minutes, 7, operation); //add the solar midnight value to the array
//                }
//                avgSolarMidnight = ArrayAverage(SolarMidnight.minutes, 7); //calculate average solar midnight from measurements over past week
//                int minute_diff = knownSolarMidnight - avgSolarMidnight; //compare with known solar midnight
//                
//                
//                clock.minutes = clock.minutes + minute_diff;
//                
//                //correction added to time
//                
//                daycount = 0;
//            }
//        }
//}
//    
//    struct month_structure SolarPerMonth;
//    SolarPerMonth.days[0] = 31;
//    SolarPerMonth.days[1] = 28;
//    SolarPerMonth.days[2] = 31;
//    SolarPerMonth.days[3] = 30;
//    SolarPerMonth.days[4] = 31;
//    SolarPerMonth.days[5] = 30;
//    SolarPerMonth.days[6] = 31;
//    SolarPerMonth.days[7] = 31;
//    SolarPerMonth.days[8] = 30;
//    SolarPerMonth.days[9] = 31;
//    SolarPerMonth.days[10] = 30;
//    SolarPerMonth.days[11] = 31;
//
//    SolarPerMonth.solarMidHours[0] = 0;
//    SolarPerMonth.solarMidHours[1] = 0;
//    SolarPerMonth.solarMidHours[2] = 0;
//    SolarPerMonth.solarMidHours[3] = 0;
//    SolarPerMonth.solarMidHours[4] = 23;
//    SolarPerMonth.solarMidHours[5] = 0;
//    SolarPerMonth.solarMidHours[6] = 0;
//    SolarPerMonth.solarMidHours[7] = 0;
//    SolarPerMonth.solarMidHours[8] = 23;
//    SolarPerMonth.solarMidHours[9] = 23;
//    SolarPerMonth.solarMidHours[10] = 23;
//    SolarPerMonth.solarMidHours[11] = 23;
//
//    SolarPerMonth.solarMidMinutes[0] = 9;
//    SolarPerMonth.solarMidMinutes[1] = 13;
//    SolarPerMonth.solarMidMinutes[2] = 8;
//    SolarPerMonth.solarMidMinutes[3] = 1;
//    SolarPerMonth.solarMidMinutes[4] = 57;
//    SolarPerMonth.solarMidMinutes[5] = 1;
//    SolarPerMonth.solarMidMinutes[6] = 5;
//    SolarPerMonth.solarMidMinutes[7] = 3;
//    SolarPerMonth.solarMidMinutes[8] = 55;
//    SolarPerMonth.solarMidMinutes[9] = 47;
//    SolarPerMonth.solarMidMinutes[10] = 46;
//    SolarPerMonth.solarMidMinutes[11] = 56;
//
//    //All timings are in GMT (+00), therefore we need to account for Daylight saving time with some of the solar midnight values
//    
////Define 4 arrays: Dawn and Dusk hours and minutes, with a predetermined size of 7 items.
////This allows us to store the times where dawn and dusk are measured by the LDR over a seven day period
////We can then take averages, calculate average solar midnight and then compare this to our known solar midnight for each month
////The difference can then be added(/subtracted) to re-calibrate/synchronise the clock
//   
//    struct array_structure { //set up time structure - we haven't actually called it yet
//        int size;
//        int count;
//        int hours[7];
//        int minutes[7];
//    };
//    
//    struct array_structure Dawn;
//        Dawn.size = 7;
//        Dawn.count = 0;                                 //ensures that Dawn is only counted once a day
//        
//        Dawn.hours[0] = 0;
//        Dawn.hours[1] = 0;
//        Dawn.hours[2] = 0;
//        Dawn.hours[3] = 0;
//        Dawn.hours[4] = 0;
//        Dawn.hours[5] = 0;
//        Dawn.hours[6] = 0;
//
//        Dawn.minutes[0] = 0;
//        Dawn.minutes[1] = 0;
//        Dawn.minutes[2] = 0;
//        Dawn.minutes[3] = 0;
//        Dawn.minutes[4] = 0;
//        Dawn.minutes[5] = 0;
//        Dawn.minutes[6] = 0;
//        
//    struct array_structure Dusk;
//        Dusk.size = 7;
//        Dusk.count = 0;                                 //ensures that Dusk is only counted once a day
//
//        Dusk.hours[0] = 0;
//        Dusk.hours[1] = 0;
//        Dusk.hours[2] = 0;
//        Dusk.hours[3] = 0;
//        Dusk.hours[4] = 0;
//        Dusk.hours[5] = 0;
//        Dusk.hours[6] = 0;
//
//        Dusk.minutes[0] = 0;
//        Dusk.minutes[1] = 0;
//        Dusk.minutes[2] = 0;
//        Dusk.minutes[3] = 0;
//        Dusk.minutes[4] = 0;
//        Dusk.minutes[5] = 0;
//        Dusk.minutes[6] = 0;
//
//
//
////    int size = 7; //size of the array to hold 7 days worth of dawn and dusk timings
////    int DawnHours[7] = {0,0,0,0,0,0,0};
////    int DawnMinutes[7] = {0,0,0,0,0,0,0};
////    int DuskHours[7] = {0,0,0,0,0,0,0};
////    int DuskMinutes[7] = {0,0,0,0,0,0,0};
//    
//    //Function to take measured Dawn and Dusk timings and add it to the 7 day moving average list.
//    //Each timing is moved down by one and the new timing is added to the end of the array.
//    
//    
//    //Defining light levels for turning LED on in dark and off in light
//    unsigned int light_threshold = 70;     // CalibrationNumber any number from 0 to 255  - Used as transition threshold light level at dawn/dusk.
//                                            //Only one light level is needed - represents both Dawn and Dusk - this is the transition from light to dark and vice versa.
//    
//    unsigned int daycount = 0; //keep track of the number of days since the last Sun synchronisation. 
//                               //this will go up to 7 where it will calculate the average solar midnight and then compare with known value
//                               //for re-calibration.
//    unsigned int previousClockDays = clock.days; //To store the current/previous day to indicate when a day has passed.
//    
//    
//     //This is to display bits to calibrate LDR for Dawn and Dusk
////      unsigned int calibration_level = ADC_getval();
////      LEDarray_disp_bin(calibration_level);
//          