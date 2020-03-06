/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#define COMPAREVALUE 50000 //set comparevalue to 50000

int main(void)
{
    set(DDRC, 6); // set PC6 as output
    teensy_clockdivide(0); //set the clock speed to 16MHz
    set(TCCR3B, WGM32); // set TIMER to mode 4
    set(TCCR3A, COM3A0); // set timer to toggle at OCR3A
    set(TCCR3B, CS31); // set the prescaler to /8
    OCR3A = COMPAREVALUE; // set timer count up to COMPAREVALUE
    /* insert the teensy initialiation here*/

    while(1); /* main loop*/
    return 0; /* never reached*/
}