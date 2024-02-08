#include <xc.h>
#include "clock.h"

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
void UpdateClock(int *s, int *m, int *h, int *d){

    //#######     TESTING MODE     ########//  if not commented out then the clock will run 1 sec = 1 hour
            *h = *s;                       // rest of the code does not need to be touched if test mode enabled
            if (*s >= 24 ){
            *s = 0; 
            }
    //--------------------------------------//
    
    if (*s >= 60 ){
        *s = 0;
        *m = *m + 1;
        }
 
        if  (*m >= 60){   // by setting this greater than or equal to 60 it ensures that we can't accidentally keep increasing seconds without increase hours
        *m = 0;
        *h = *h + 1;
        }
        
        if (*h >= 24){
        *h = 0;
        *d = *d + 1;
        }
}
