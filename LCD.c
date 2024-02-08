#include <xc.h>
#include <stdio.h>
#include "LCD.h"


/************************************
 * Function to toggle LCD enable bit on then off
 * when this function is called the LCD screen reads the data lines
************************************/
void LCD_E_TOG(void)
{
	//turn the LCD enable bit on
     LATCbits.LATC2 = 1;
	__delay_us(2); //wait a short delay
     LATCbits.LATC2 = 1;
	//turn the LCD enable bit off again
}

/************************************
 * Function to set the 4-bit data line levels for the LCD
************************************/
void LCD_sendnibble(unsigned char number)
{
    if (number & 0b0001) {LATBbits.LATB3=1;} else {LATBbits.LATB3=0;} 
    if (number & 0b0010) {LATBbits.LATB2=1;} else {LATBbits.LATB2=0;}
    if (number & 0b0100) {LATEbits.LATE3=1;} else {LATEbits.LATE3=0;}
    if (number & 0b1000) {LATEbits.LATE1=1;} else {LATEbits.LATE1=0;}
    
	//set the data lines here (think back to LED array output)

    LCD_E_TOG();			//toggle the enable bit to instruct the LCD to read the data lines
    __delay_us(5);      //Delay 5uS
}


/************************************
 * Function to send full 8-bit commands/data over the 4-bit interface
 * high nibble (4 most significant bits) are sent first, then low nibble sent
************************************/
void LCD_sendbyte(unsigned char Byte, char type)
{
    LCD_RS = type;
    
    LCD_sendnibble(Byte>>4); //Bit shift by 4 - Sends the 4 high bits of the 8bit
    LCD_sendnibble(Byte); //Sends the 4 least significant bits to be sent to the LCD display
    
    // set RS pin whether it is a Command (0) or Data/Char (1) using type argument
    // send high bits of Byte using LCDout function
    // send low bits of Byte using LCDout function
	
    __delay_us(50);               //delay 50uS (minimum for command to execute)
}

/************************************
 * Function to initialise the LCD after power on
************************************/
void LCD_Init(void)
{

    //Define LCD Pins as Outputs 
    //set all pins low (might be random values on start up, fixes lots of issues)
// LCD is connected to pins that I can't read on the PIC board // LCD is connected to pins that I can't read on the PIC board 
// look at the manual
    
//LCD pin as ouutputs 
     //Define LCD Pins as Outputs
  
    
    TRISBbits.TRISB3 = 0;  //pin 12 DB4
    //    LATBbits.LATB3 = 0;
    LCD_DB4 = 0;
    
    TRISBbits.TRISB2 = 0;    //pin 12 DB5
//    LATBbits.LATB2 = 0;
    LCD_DB5 = 0;
    
    TRISEbits.TRISE3 = 0;    //pin 13 DB6
//    LATEbits.LATE3 = 0;
    LCD_DB6 = 0;
    
    TRISEbits.TRISE1 = 0;    //pin 14 DB7
//    LATEbits.LATE1 = 0;
    LCD_DB7 = 0;
    
    TRISCbits.TRISC2 = 0; //Enable
    LCD_E = 0;             
    
    TRISCbits.TRISC6 = 0; //RS H:Data L:Instruction mode
    LCD_RS = 0;
    
    __delay_ms(150); 
    LCD_sendnibble(0b0011);
    __delay_ms(5); 
    LCD_sendnibble(0b0011);
    __delay_us(150); 
    LCD_sendnibble(0b0011);
    __delay_us(150);
    LCD_sendnibble(0b0010); //Up until here the LCD is in 8bit mode
    __delay_us(150);
    LCD_sendbyte(0b00101100,0); //Now in 4 bit mode so byte is split up into 2 when sent to the LCD display as is done in the LCD_sendbyte function
    __delay_us(100); 
    LCD_sendbyte(0b00001000,0);
    __delay_ms(100); 
    LCD_sendbyte(0b00000001,0);
    __delay_ms(5);
    LCD_sendbyte(0b00000110,0);
    __delay_ms(100); //delay to allow initialisation to finish
    LCD_sendbyte(0b00001100,0); //Turns the display back on after initialisation 
    __delay_ms(100);
      
	// first Function set should be sent with LCD_sendnibble (the LCD is in 8 bit mode at start up)
	// after this use LCD_sendbyte to operate in 4 bit mode

	//remember to turn the LCD display back on at the end of the initialisation (not in the data sheet)
}

/************************************
 * Function to set the cursor to beginning of line 1 or 2
************************************/
void LCD_setline (char line)
{
    //Send 0x80 to set line to 1 (0x00 ddram address)
    //Send 0xC0 to set line to 2 (0x40 ddram address)
    if (line == 2){
        LCD_sendbyte(0xC0,0);
    }
    //Send 0x80 to set line to 1 (0x00 ddram address)
    if (line==1){
        LCD_sendbyte(0x80,0);
    }
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_sendstring(char *string)
{
	//code here to send a string to LCD using pointers and LCD_sendbyte function
    while(*string != 0){  // While the data pointed to isn?t a 0x00 do below (strings in C must end with a NULL byte) 
		LCD_sendbyte(*string++,1); 	//Send out the current byte pointed to and increment the pointer
	}
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_scroll(void)
{
	LCD_sendbyte(0b00011000,0); //Function to implement scroll to the left 
    //code here to scroll the text on the LCD screen
}

/************************************
 * Function takes a ADC value and works out the voltage to 2 dp
 * the result is stored in buf as ascii text ready for display on LCD
 * Note result is stored in a buffer using pointers, it is not sent to the LCD
************************************/
void ADC2String(char *buf, unsigned int ADC_val){
	//code to calculate the inegeter and fractions part of a ADC value
	// and format as a string using sprintf (see GitHub readme)
    //code to calculate the inegeter and fractions part of a ADC value
	// and format as a string using sprintf (see GitHub readme)
    unsigned int int_part = 0;
    char frac_part = 0;  //Sets up variabled to be used
    
    int_part = ADC_val/77; // i.e. 255 / 3.3V = 77 Scales val to 3.3v
    frac_part = (ADC_val*100)/77 - int_part*100; //Scaled to 3.3 v and *100 to get the decimal part as the first 2 digits
    sprintf(buf,"Time: %02d:%02d:%04d",12,16,35); //Sets buf string to take the first int part value and the first 2 values in the frac part variable
    LCD_sendstring(buf); //Prints buf value in LCD display
    
}
