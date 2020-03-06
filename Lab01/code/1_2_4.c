/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file

int main(void)
{   
    set(DDRC, 6); // set PC6 as output
    teensy_clockdivide(0); // set the clock speed
    int t; // create a variable that change the time teensy wait
    t = 3000; // set the cycle time
    float duty_circle; // create a variable that change the duty circle
    duty_circle = 0.2; // set duty circle
    /* insert the teensy initialization here*/

    for(;;)
    {
        /* insert your main loop here*/
        toggle(PORTC,6); // change the state of teensy, Turn ON the LED
        teensy_wait((duty_circle*t); // The teensy ON  for (duty_circle)*t ms
        toggle(PORTC,6); // change the state of teensy, Yurn OFF the LED
        teensy_wait((1-duty_circle)*t); // The teensy OFF for (1-duty_circle)*t ms
    }
    return 0; /* never reached*/
}
