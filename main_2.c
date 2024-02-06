#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "LEDarray_3.h"
#include "ADC_3.h"
#include "timers.h"
#include "comparator.h"
#include "interrupts.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

//#include "interrupts1.h"
//#include "timers1.h"
// pin A3 for LDR

void main(void) 
{   
    LEDarray_init();        //setting up the LED array 
    Timer0_init();
    Interrupts_init();   
    ADC_init();             // setting up the ADC
    
    // calibration steps - maybe move to a .c and .h file 
    unsigned int maxLight = 170;            // CalibrationNumber any number from 0 to 255  - 105 worked for my room at home - 185 worked in the LAB
    unsigned int minLight = 70;             // CalibrationNumber any number from 0 to 255  - used to make sure finger on the LED makes it go dark
    unsigned int range;
    unsigned int step;
    range = maxLight - minLight ;
    step = range/9 ; // spreads the values acrosse the 9 avaliable LEDs 
    unsigned int MaxVal = 0;
    unsigned int counter = 0;
    
    
    while (1) {
 
//    if(ADC_getval() > MaxVal){
//        MaxVal = ADC_getval();
//    }  
//    
//    else{
//        counter++;
//        __delay_ms(10);
//        if (counter>100){
//            MaxVal = MaxVal - step;
//            counter=0;
//        }
//    }
    
//    LEDarray_disp_PPM(ADC_getval(), MaxVal, maxLight, minLight, step) ;
    
    LEDarray_disp_bin(get16bitTMR0val()) ;// 
//    LEDarray_disp_bin(get16bitTMR0val()) ;// 
   
    }
}
