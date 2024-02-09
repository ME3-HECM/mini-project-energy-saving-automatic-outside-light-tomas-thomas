# Mini project - Tomas & Thomas - Energy saving automatic outside light
read me content added by Tomas Lashmar and Thomas Young

To solve this project 2 major elements were created in addition to the lab material covered.
1. A global clock
2. Sychonisation of the global clock according to dusk and dawn readings from the LDR

## How to use the light and the code?
First input the exact time when the light will be powered on and thus intialise the clock.
Second enjoy your clock :)

## How do we keep track of the time? 
To solve this issue a global clock was created. The clock has been written to account for leap years 
- The clock accounts for leap years
- The clock accounts for century leap years
- It will switch to daylight savings time on the last sunday in March
- It will switch off daylight savings on the last sunday in October
The clock runs in the "clock.c" file and takes 9 inputs
1. Seconds
2. Minutes
3. Hours
4. Days
5. Day of the Week (DoW)
6. Months
7. Years
8. Day light Saving Time State (DSTstate)
9. Testmode

Whilst the clock is written in a "clock.c" file the intialisation values of the intial time is in the "main.c" value and as such pointers were used to change the value of seconds, mins, days, etc. Additionally structures were used to contain the time of day values making it more effeicnt than typical integers. It was a fun learning experience and was very helpful.

The clock functions by using a calibrated timer using the Timer0 function developed in LAB 3. The timer is calibrated so that it will overflow and trigger an interupt every second. This is done by starting the timer from an set value (TMR0H = 0b00001011; TMR0L = 0b11011100;) which causes the timer to overflow in a second of real time due to the given the 1:256 prescaler on the 64Mhz chip.

The interupt sequence was used to then increment a global variable "GLOBALsecs". This allows us to then use various if statements cascaded together to count minutes then hours, then days, etc. This is how the clock keeps track of time. >= statements were used when deciding if to increase the minutes count when the count got to 60 instead of == statements. This was done to provide a fail safe if somehow the seconds counter was incremented twice therefore skipping 60 and missing the == 60 if loop. This would break our clock and this way we would always keep counting.

There is another if loops to make sure the days turn into the correct months and after 12 months we turn a year. This uses a list and the month coutner to determine the exact day it needs to swtich to the correct month. Leap years are accounted for by overiding the month day in February if the leap year or century leap year conditions are met.

## Daylight Savings Time
The Days of the week function was introduced to calculated the daylight savings time as this is determined by the last sunday in October and March.
```
if((*DSTstate == 0) && (*months == 3) && (*days >= 25) && (*DoW == 7) && ( *hours >= 1) ){ 
           //moves the clocks forwards by 1 hour 
        *hours = *hours + 1; 
        if (TestMode == 1){ 
            *seconds = *seconds + 1;
        }
        *DSTstate = 1;
```
This code checks 5 states: 
- is DST on yet?
- are we in March?
- is it the last week of March?
- is it the sunday in the last week?
- and is it 1am when DST switches.
  
The code then shifts the hours forward by an hour (as seen in the LCD display) if these conditions are met. The "TestMode if statement" is to enable the hours counter to increment at the same pace as real time seconds during TestMode - we had lots of issues troubleshooting this but it now works. The final line "*DSTstate = 1;" makes us enter our DSTstate which prevents the loop from repeatedly running and adding DST hours without the need to specify a specifc time (this could cause an issue if the timer skipped this value and for 6 months the clock would be out of sync).


The same appraoch was used to get of day light savings time when reach the last Sunday in the last week of October.
```
 if((*DSTstate==1) && (*months == 10) && (*days >= 25) && (*DoW == 7) && (*hours == 2) ){ //is daylight savings on, is it march, is it the last week of march, is it a sunday, is it 1 o'clock? if yes then turn DST on
        *hours = *hours - 1;        //moves the clocks forwards by 1 hour
        if (TestMode == 1){        //due to the way seconds and hours are linked in the test mode this is necessary to do the daylight savings time
            *seconds = *seconds - 1;
        }
        *DSTstate = 0;          // turn daylight savings on 
    }
```

## Sun Syncing
this was fun - over to Tommy



# Orignal Assignment Brief
[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/hkDdx-pz)


## Learning outcomes

The principal learning objectives for this project are:

- Implement a working microprocessor based system to achieve a more complex real world task
- Develop your ability to independently plan, organise and structure your code 
- Improve your grasp of the C language and writing your own functions

## Brief

Outside lights that respond to ambient light levels are commonplace (i.e. street lights, porch lights, garden lights etc). These types of lights switch on at dusk and then turn off at dawn. However, energy and money can be saved if these lights are switched off during the small hours of the morning (for example, between 1am and 5am), when there are very few people around. Many councils in the UK have implemented/trialled this idea for street lights (https://www.bbc.co.uk/news/uk-england-derbyshire-16811386). Your task is to use the knowledge of microcontrollers and hardware that you have gained in labs 1-3 from this module to develop a fully automated solution.

## Specification
Design and program a device that meets the following requirements:

1. Monitors light level with the LDR and turns on an LED in low light conditions (i.e. night-time) and off in bright conditions (i.e. daytime)
1. Displays the current hour of day on the LED array in binary
1. Turns the light off between approx. 1am and 5am
1. Adjusts for daylight savings time
1. Maintain synchronicity with the sun indefinitely
1. Be fully automatic (requires zero maintenance after installation)

Please use this GitHub repo to manage your software development and submit your mini project code.

## Supplementary information and help
At first the task may seem quite straightforward but there are several points that often prove more tricky. The first is how to test code during development? You could test in real world conditions but you would be limited to one test cycle per day and this would severely slow down your development and debugging progress. To get around this you could implement a "testing mode" and pretend that a day lasts 24 seconds. This could be done using a #define directive to switch between "normal" and "testing" modes for your code.

Adjusting for daylight savings time is not too tricky. The clocks always change (in the UK) on the last Sunday in March (they go forward an hour) and the last Sunday in October (they go back an hour). One method of achieving this is to initialise what day it is when device is first switched on (using manual input) and then keep track of the days that pass and what the day of the week it is. Another method might be to automatically figure out what time of year it is (see below). Also don't forget about leap years! 

No clock is perfect, they can all run slightly fast/slow and can by influenced be external factors such as temperature. Ultimately this will result in drift over time and eventually the time will drift so far out of sync with real time that it is meaningless. For the purposes of our device the main requirement is that it remains in sync with the sun. You could use light from the sun to keep your clock in sync. Although the length of daylight varies considerably during the year, the midpoint between dusk and dawn only varies by a few minutes. This is termed solar midnight approx. 12am or solar noon approx. 12pm. One method of staying in sync with the sun is to use the LDR and record/calculate when these times occur and adjust your clock accordingly. The length of daylight also tells us information about what time of year it is and can be used to help us know when to adjust for daylight savings time.

![Day length](gifs/day-length-london.jpg)
http://wordpress.mrreid.org/2010/10/31/why-change-the-clocks/

## Additional Content added
The plan before work got started - for reference 

1. Monitors light level with the LDR and turns on an LED in low light conditions (i.e. night-time) and off in bright conditions (i.e. daytime)
    - LDR sensitive should be straight forward
1. Displays the current hour of day on the LED array in binary
    - internal clock
    - if clock overflows in each second
    - add to the counter counter resets every 24 hours?
1. Turns the light off between approx. 1am and 5am
    - requires a global clock
1. Adjusts for daylight savings time
    - parameter for determining which part of the year we are in
1. Maintain synchronicity with the sun indefinitely
    - needs a recalibration feature
    - track maximum darkness or other light source  for 7 days
    - set maximum time of clock according to solar profile 
1. Be fully automatic (requires zero maintenance after installation)
    - we will need an instaliation date in the code 





