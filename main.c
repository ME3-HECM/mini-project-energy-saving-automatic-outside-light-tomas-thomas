/*
 * File:   main.c
 * Author: tjhla
 *
 * Created on 02 February 2024, 11:16
 */
// ###############################################################pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

// ##############################################################

// include all .h files here
#include <xc.h>
#include "LEDarray.h"
#include "ADC.h"
#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

// ASUMPTIONS
// assuming light levels are binary - will output to an LED on the board - RH3 to show the street light is on 
    // potential to have street lights scale with the LED on the side as sun rises 


void main(void) 
{
    LEDarray_init();        //setting up the LED array 
    ADC_init();             // setting up the ADC
    
    unsigned int maxLight = 170;            // CalibrationNumber any number from 0 to 255  - 105 worked for my room at home - 185 worked in the LAB
    unsigned int minLight = 70;             // CalibrationNumber any number from 0 to 255  - used to make sure finger on the LED makes it go dark
    unsigned int range;
    unsigned int step;
    range = maxLight - minLight ;
    step = range/9 ; // spreads the values acrosse the 9 avaliable LEDs 
    
    unsigned int MaxVal = 0;
    unsigned int counter = 0;
    while (1) {
    
    
     // I want to drop the input 
        
    if(ADC_getval() > MaxVal){
        MaxVal = ADC_getval();
    }  
    
    else{
        counter++;
        __delay_ms(10);
        if (counter>100){
            MaxVal = MaxVal - step;
            counter=0;
        }
    }
    
    LEDarray_disp_PPM(ADC_getval(),MaxVal, maxLight, minLight, step) ;
   
    }
=======
void main(void) {
    
    
    //this is a test file
    
    //does it work?
    return;

}
