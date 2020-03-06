/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#define COMPAREVALUE 50000 //set comparevalue to 100

int main(void)
{
    set(DDRC, 6); // set PC6 as output
    TCCR3B = 0x01; // Turn on counter (no prescale)
    set(TCCR3B, WGM32); // set TIMER to mode 4
    set(TCCR3A, COM3A0); // set timer to toggle at OCR3A
    OCR3A = COMPAREVALUE; // set timer count up to COMPAREVALUE
    /* insert the teensy initialiation here*/

    while(1); /* main loop*/
    return 0; /* never reached*/
}
