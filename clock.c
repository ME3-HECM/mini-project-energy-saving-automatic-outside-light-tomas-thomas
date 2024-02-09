#include <xc.h>
#include "clock.h"
#include "seconds.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UpdateClock(int *seconds, int *minutes, int *hours, int *days, int *DoW, int *months, int *years, int *DSTstate, int TestMode){

    int DaysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //this should be in an intialisation function 
    
    if (TestMode == 1){
       *hours = *seconds;    //every IRL second = 1 hour on the clock
        if (*seconds >= 24 ){   // whilst in this mode we need to reset the seconds counter at 24 to be like the days
            *seconds = 0;       // reset to zero
        } 
    }
    
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
        *DoW = *DoW + 1;
    }
    
    if  (*DoW >= 8){  
        *DoW = 1;
    }
    
    if((*DSTstate == 0) && (*months == 3) && (*days >= 25) && (*DoW == 7) && ( *hours >= 1) ){ //is daylight savings on, is it march, is it the last week of march, is it a sunday, is it 1 o'clock? if yes then turn DST on
           //moves the clocks forwards by 1 hour 
        *hours = *hours + 1; //not sure why this doesn't work in fast mode ??
        if (TestMode == 1){ //due to the way seconds and hours are linked in the test mode this is necessary to do the daylight savings time
            *seconds = *seconds + 1;
        }
        *DSTstate = 1;          // turn daylight savings on 
        
    }
    
    if((*DSTstate==1) && (*months == 10) && (*days >= 25) && (*DoW == 7) && (*hours == 2) ){ //is daylight savings on, is it march, is it the last week of march, is it a sunday, is it 1 o'clock? if yes then turn DST on
        *hours = *hours - 1;    //moves the clocks forwards by 1 hour
        if (TestMode == 1){//due to the way seconds and hours are linked in the test mode this is necessary to do the daylight savings time
            *seconds = *seconds - 1;
        }
        *DSTstate = 0;          // turn daylight savings on 
    }
     
    // Dealing with leap years         
    if ((*years)%4 == 0 && *months == 2){  //check that we're not in a leap year and in FEB.
       DaysInMonth[1] = 29;               // if so add the extra day
    }
            
    // Dealing with leap years but its not a leap year because its a century that isn't / 400       
    if (*years%100 == 0 && *months == 2 && *years%400 != 0){   //2000 is a leap year, however 1900 isn't a leap year accounting for
            DaysInMonth[1] = 28; 
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
 