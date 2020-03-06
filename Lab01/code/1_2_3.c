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
    t = 5; // set the cycle time
    /* insert the teensy initialization here*/

    for(;;)
    {
        /* insert your main loop here*/
        toggle(PORTC,6); // change the state of teensy
        teensy_wait(t); // The teensy delay t ms
    }
    return 0; /* never reached*/
}
