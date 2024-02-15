#include <xc.h>
#include "LEDarray.h"

/************************************
/ LEDarray_init
/ Function used to initialise pins to drive the LEDarray
************************************/
void LEDarray_init(void)
{
    //set up TRIS registers for pins connected to LED array
	//set initial output LAT values (they may have random values when powered on)
         
    // we've got 9 LEDS - 512 numbers 
    
    //LED 1 - RGO
    TRISGbits.TRISG0 = 0;   // setting the input/output to output 
    LATGbits.LATG0 = 0;     // Setting the Left LED as ON - (0=off, 1=on)
    //LED 2 - RG1
    TRISGbits.TRISG1 = 0;   // setting the input/output to output
    LATGbits.LATG1 = 0;     // Setting the Left LED as ON - (0=off, 1=on)
    //LED 3 - RA2
    TRISAbits.TRISA2 = 0;   // setting the input/output to output
    LATAbits.LATA2 = 0;     // Setting the Left LED as ON - (0=off, 1=on)
    //LED 4 - RF6
    TRISFbits.TRISF6 = 0;   // setting the input/output to output
    LATFbits.LATF6 = 0;     // Setting the Left LED as ON - (0=off, 1=on)
    //LED 5 - RA4
    TRISAbits.TRISA4 = 0;   // setting the input/output to output
    LATAbits.LATA4 = 0;     // Setting the Left LED as ON - (0=off, 1=on)
    //LED 6 - RA5
    TRISAbits.TRISA5 = 0;   // setting the input/output to output
    LATAbits.LATA5 = 0;     // Setting the Left LED as ON - (0=off, 1=on)
    //LED 7 - RF0
    TRISFbits.TRISF0 = 0;   // setting the input/output to output
    LATFbits.LATF0 = 0;     // Setting the Left LED as ON - (0=off, 1=on)
    //LED 8 - RB0
    TRISBbits.TRISB0 = 0;   // setting the input/output to output
    LATBbits.LATB0 = 0;     // Setting the Left LED as ON - (0=off, 1=on)
    //LED 9 - RB1
    TRISBbits.TRISB1 = 0;   // setting the input/output to output
    LATBbits.LATB1 = 0;     // Setting the Left LED as ON - (0=off, 1=on)
    
}

/************************************
/ LEDarray_disp_bin
/ Function used to display a number on the LED array in binary
************************************/
void LEDarray_disp_bin(unsigned int number) // goal is take a number and display it on the board, bit shifting is a way to make it better 
{
//    LATDbits.LATD7 = !LATDbits.LATD7;
	//some code to turn on/off the pins connected to the LED array
    if (number & 0b000000001){ LATGbits.LATG0 = 1 ; } 
    else{LATGbits.LATG0 = 0;}
    
    if (number & 0b000000010){ LATGbits.LATG1 = 1 ; } 
    else{LATGbits.LATG1 = 0;}
    
    if (number & 0b000000100){ LATAbits.LATA2 = 1 ; } 
    else{LATAbits.LATA2 = 0;}
    
    if (number & 0b000001000){ LATFbits.LATF6 = 1 ; } 
    else{LATFbits.LATF6 = 0;}
    
    if (number & 0b000010000){ LATAbits.LATA4 = 1 ; } 
    else{LATAbits.LATA4 = 0;}
    
    if (number & 0b000100000){ LATAbits.LATA5 = 1 ; } 
    else{LATAbits.LATA5 = 0;}
    
    if (number & 0b001000000){ LATFbits.LATF0 = 1 ; } 
    else{LATFbits.LATF0 = 0;}
    
    if (number & 0b010000000){ LATBbits.LATB0 = 1 ; } 
    else{LATBbits.LATB0 = 0;}
    
    if (number & 0b100000000){ LATBbits.LATB1 = 1 ; } 
    else{LATBbits.LATB1 = 0;}
    
	//if statements and bit masks can be used to determine if a particular pin should be on/off
	//see Readme.md for examples
}

/************************************
/ Function LEDarray_disp_dec
/ Used to display a number on the LEDs
/ where each LED is a value of 10
************************************/
void LEDarray_disp_dec(unsigned int number)
{
	unsigned int disp_val;
    
    if (number >= 1){ LATGbits.LATG0 = 1 ; } 
    else{LATGbits.LATG0 = 0;}
    
    if (number >= 2){ LATGbits.LATG1 = 1 ; } 
    else{LATGbits.LATG1 = 0;}
    
    if (number >= 3){ LATAbits.LATA2 = 1 ; } 
    else{LATAbits.LATA2 = 0;}
    
    if (number >= 4){ LATFbits.LATF6 = 1 ; } 
    else{LATFbits.LATF6 = 0;}
    
    if (number >= 5){ LATAbits.LATA4 = 1 ; } 
    else{LATAbits.LATA4 = 0;}
    
    if (number >= 6){ LATAbits.LATA5 = 1 ; } 
    else{LATAbits.LATA5 = 0;}
    
    if (number >= 7){ LATFbits.LATF0 = 1 ; } 
    else{LATFbits.LATF0 = 0;}
    
    if (number >= 8){ LATBbits.LATB0 = 1 ; } 
    else{LATBbits.LATB0 = 0;}
    
    if (number >= 9){ LATBbits.LATB1 = 1 ; } 
    else{LATBbits.LATB1 = 0;}
	
	//some code to manipulate the variable number into the correct
	//format and store in disp_val for display on the LED array

//	LEDarray_disp_bin(disp_val); 	//display value on LED array
}


/************************************
/ LEDarray_disp_PPM
/ Function used to display a level on the LED array with peak hold
/ cur_val is the current level from the most recent sample, and max is the peak value for the last second
/ these input values need to calculated else where in your code
************************************/

void LEDarray_disp_light(unsigned int number, unsigned int maxLight, unsigned int minLight, unsigned int step)
{
   
    if (number > minLight + step*1 ){ LATGbits.LATG0 = 1 ; } 
    else{LATGbits.LATG0 = 0;}

    if (number > minLight + step*2 ){ LATGbits.LATG1 = 1 ; } 
    else{LATGbits.LATG1 = 0;}

    if (number > minLight + step*3 ){ LATAbits.LATA2 = 1 ; } 
    else{LATAbits.LATA2 = 0;}

    if (number > minLight + step*4 ){ LATFbits.LATF6 = 1 ; } 
    else{LATFbits.LATF6 = 0;}

    if (number > minLight + step*5 ){ LATAbits.LATA4 = 1 ; } 
    else{LATAbits.LATA4 = 0;}

    if (number > minLight + step*6 ){ LATAbits.LATA5 = 1 ; } 
    else{LATAbits.LATA5 = 0;}

    if (number > minLight + step*7 ){ LATFbits.LATF0 = 1 ; } 
    else{LATFbits.LATF0 = 0;}

    if (number > minLight + step*8 ){ LATBbits.LATB0 = 1 ; } 
    else{LATBbits.LATB0 = 0;}

    if (number > minLight + step*9 ){ LATBbits.LATB1 = 1 ; } 
    else{LATBbits.LATB1 = 0;}

}

// ########################################
void LEDarray_disp_PPM(unsigned int numberIn, unsigned int MaxVal, unsigned int maxLight, unsigned int minLight, unsigned int step)
// function displays a single value of LED MaxVAL
// and displays shows wave output based on numberIn
{

// doing the single LED display 
    
    if ((MaxVal > minLight + step*1) && (MaxVal <= minLight + step*2)){ LATGbits.LATG0 = 1 ; } 
    else{LATGbits.LATG0 = 0;}

    if ((MaxVal > minLight +step*2) && (MaxVal <= minLight +step*3)){ LATGbits.LATG1 = 1 ; } 
    else{LATGbits.LATG1 = 0;}
    
    if ((MaxVal > minLight +step*3) && (MaxVal <= minLight +step*4)){ LATAbits.LATA2 = 1 ; } 
    else{LATAbits.LATA2 = 0;}
    
    if ((MaxVal > minLight +step*4) && (MaxVal <= minLight +step*5)){ LATFbits.LATF6 = 1 ; } 
    else{LATFbits.LATF6 = 0;}
    
    if ((MaxVal > minLight +step*5) && (MaxVal <= minLight +step*6)){ LATAbits.LATA4 = 1 ; } 
    else{LATAbits.LATA4 = 0;}
    
    if ((MaxVal > minLight +step*6) && (MaxVal <= minLight +step*7)){ LATAbits.LATA5 = 1 ; } 
    else{LATAbits.LATA5 = 0;}
    
    if ((MaxVal > minLight +step*7) && (MaxVal <= minLight +step*8)){ LATFbits.LATF0 = 1 ; } 
    else{LATFbits.LATF0 = 0;}
        
    if ((MaxVal > minLight +step*8) && (MaxVal <= minLight +step*9)){ LATBbits.LATB0 = 1 ; } 
    else{LATBbits.LATB0 = 0;}
        
    if (MaxVal > minLight +step*9){ LATBbits.LATB1 = 1 ; } 
    else{LATBbits.LATB1 = 0;}

    // displaying the wave 
    if (numberIn > minLight + step*1 ){ LATGbits.LATG0 = 1 ; } 
    
    if (numberIn > minLight + step*2 ){ LATGbits.LATG1 = 1 ; } 

    if (numberIn > minLight + step*3 ){ LATAbits.LATA2 = 1 ; } 

    if (numberIn > minLight + step*4 ){ LATFbits.LATF6 = 1 ; } 

    if (numberIn > minLight + step*5 ){ LATAbits.LATA4 = 1 ; } 

    if (numberIn > minLight + step*6 ){ LATAbits.LATA5 = 1 ; } 

    if (numberIn > minLight + step*7 ){ LATFbits.LATF0 = 1 ; } 

    if (numberIn > minLight + step*8 ){ LATBbits.LATB0 = 1 ; } 

    if (numberIn > minLight + step*9 ){ LATBbits.LATB1 = 1 ; } 
}

