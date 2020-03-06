/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#define COMPAREVALUE 100 //set comparevalue to 100

int main(void)
{
    set(DDRC,6); // set PC6 as output
    teensy_clockdivide(0); // set the teensyclock to 16MHz
    set(TCCR3B, CS30); // set the prescaler to /1
    set(TCCR3B, WGM33); // set timer to mode 14
    set(TCCR3B, WGM32); // set timer to mode 14
    set(TCCR3A, WGM31); // set timer to mode 14
    set(TCCR3A, COM3A1); // clear at OCR3A and set at rollover
    ICR3 = COMPAREVALUE; // set PWM up to COMPAREVALUE
    int i; // set the step of PWM
    int upTime; // create a time variable for LED to be fully ON
    upTime = 300; // set upTime to 0.3 sec
    int downTime; // create a time variable for LED to be fully OFF
    downTime = 700; // set downTime to 0.7 sec
    float intensityIncrease; // create waiting time for intensity increase
    intensityIncrease = upTime/COMPAREVALUE; // intensityIncrease should be 3 ms;
    float intensityDecrease; // create waiting time for intensity decrease
    intensityDecrease = downTime / COMPAREVALUE; // intensityDecrease should be 7 ms;
    /* insert your initialization here*/

    for(;;) // the main loop
    {
        for(i = 0; i < ICR3; i++) // loop for the intensity increase
        {
            OCR3A = i; // set the duty circle of LED
            teensy_wait(intensityIncrease); // teensy wait at a certain time at a certain duty circle
        }
        for(i = COMPAREVALUE; i >0 ; i--) // loop for the intensity decrease
        {
            OCR3A = i; // set the duty circle of LED
            teensy_wait(intensityDecrease); // teensy wait at a certain time at a certain duty circle
        }
    }
    return 0; /* never reach*/
}