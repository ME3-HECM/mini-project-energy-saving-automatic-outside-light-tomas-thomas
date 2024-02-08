#include <xc.h>
#include "synchronisation.h"

//Function to take measured Dawn and Dusk timings and add it to the 7 day moving average list.
//Each timing is moved down by one and the new timing is added to the end of the array.
int ArrayAppend(int arrayTime[], int size, int Time) { //take the original list and the hours and minutes of the newly measured dawn/dusk
        
        for (int i = 1; i <= size-1; i++) { //looping through each item in the array starting at the second item, and removing the first item
            
            if (i = 6) { //for the last item, now add the measured hours and minutes data
                arrayTime[i-1] = arrayTime[i]; 
                arrayTime[i] = Time;
            }
            
            else {
                arrayTime[i-1] = arrayTime[i]; //shift all the items along by one to simulate the seven day moving average - this process removes the first item (more than 7 days ago).
            }
        }
        
        return arrayTime;
    }  


