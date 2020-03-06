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
    float duty_circle; // create a duty circle variable
    duty_circle = 0; // set duty circle to 0;
    OCR3A = duty_circle*COMPAREVALUE; // set OCR3A equals to duty_circle*COMPAREVALUE
    /* insert the initialization here*/

    while(1); // main loop
    return 0; /* never reach*/
}