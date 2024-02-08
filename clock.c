#include <xc.h>
#include "clock.h"
#include "seconds.h"

void clock_init(void){
//    int time_vals[4] = {0, 0, 0, 0} ;
//#define wazz time_vals[0]

    //int time_vals[4] = {seconds, minutes, hours, days}
//    
//    struct time_structure { //set up time structure
//        int seconds;
//        int minutes;
//        int hours;
//        int days;
//    };
//
//    struct time_structure clock = { 0, 0, 0, 0};    //creates clock, which is of the structure time_structure
//    // set the intial time 
//    
//    int *ptra;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UpdateClock(int *seconds, int *minutes, int *hours, int *days, int *months, int *years){

    int DaysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //this should be in an intialisation function 
    
    
    //#######     TESTING MODE     ########//  if not commented out then the clock will run 1 sec = 1 hour
            *hours = *seconds;                       // rest of the code does not need to be touched if test mode enabled
            if (*seconds >= 24 ){
                *seconds = 0;
                *hours = 0; 
                *days = *days + 1;
            }
    //-----------------------------~~~~---------//     
    
    if (*seconds >= 60 ){
        *seconds = 0;
        *minutes = *minutes + 1;
    }
 
    if  (*minutes >= 60){   // by setting this greater than or equal to 60 it ensures that we can't accidentally keep increasing seconds without increase hours
        *minutes = 0;
        *hours = *hours + 1;
    }
            
    if  (*hours >= 24){   // by setting this greater than or equal to 60 it ensures that we can't accidentally keep increasing seconds without increase hours
        *hours = 0;
        *days = *days + 1;
    }
     
    // Dealing with leap years         
    if ((*years)%4 == 0 && *months == 2){  //check that we're not in a leap year and in FEB.
       DaysInMonth[1] = 29;               // if so add the extra day
    }
            
    // Dealing with leap years but its not a leap year because its a century that isn't / 400       
    if (*years%100 == 0 && *months == 2){   //2000 is a leap year, however 1900 isn't a leap year accounting for
        if(*years%400 != 0){
            DaysInMonth[1] = 28; 
        }  
    }
    
    //resetting the calendar in December to go back to Jan 
    if (*days > (DaysInMonth[*months-1] ) ) {
        *days = 1; // Reset days counter
        *months = *months + 1 ;       // Increment month counter  
        
        if (*months >= 12+1) { // Reset month counter at the end of the year
            *months = 1;       //sets month back to 1
            *years = *years + 1; //add a year 
        }
    }        
}        
 