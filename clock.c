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
void UpdateClock(int *seconds, int *minutes, int *hours, int *days){

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
        
    if (*hours >= 24){
    *hours = 0;
    *days = *days + 1;
    }
}
