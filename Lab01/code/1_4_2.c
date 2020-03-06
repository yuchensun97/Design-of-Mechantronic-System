/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#define COMPAREVALUE 100 //set comparevalue to 100

// declare the subroutine
int lighting(float); //Create a subroutine that control the intensity of LED

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
    float intensity_ls[2] = {1, 0.5}; // create a variable for LED intensity
    int j; // create a variable for the postion in intensity_ls
    float LEDintensity; // create a variable for intensity for LED
    /* insert the teensy initialization here */

    for(;;) /* insert the main loop*/
    {
        for (j = 0; j < 2; j++) // the pulsing loop
        {
            LEDintensity =intensity_ls[j]; // set LED intensity
            lighting(LEDintensity); // call the subroutine to control the LED intensity
        }
        teensy_wait(2000); // Let LED be off for 2 sec
    }
    return 0;
}

int lighting(float intensity)
{
    int i; // set the step of PWM
    int upTime = 100; // create a time variable for LED to be fully ON
    int downTime = 400; // create a time vatiable for LED to be full OFF
    int intensityIncrease; // create waiting time variable for intensity increase
    intensityIncrease = upTime / COMPAREVALUE; // waiting time for intensity fully increase should be 1 ms
    int intensityDecrease; //  create waiting time variable for intensity decrease
    intensityDecrease = downTime / COMPAREVALUE; // waiting time for intensity fully decrease should be 4 ms

    for ( i = 0; i < intensity*COMPAREVALUE; i++) // loop for intensity increase
    {
        OCR3A = i; // set the duty circle of LED
        teensy_wait(intensityIncrease*(1/intensity)); // teensy wait at a certain time at a certain duty circle
    }
    for( i = intensity*COMPAREVALUE; i > 0; i--) // loop for intensity decrease
    {
        OCR3A = i; // set the duty circle of LED
        teensy_wait(intensityDecrease*(1/intensity)); // teensy wait at a ceertain time at a certain duty circle
    }

    return 0;
}
