/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file

int main(void)
{
    set(DDRD, 1); // set PD1 as output
    clear(DDRC, 7); // clear value in PC7
    clear(PORTD, 1); // Take the output PD1 low
    /* insert teensy initialization here*/

    while(1)  // the main loop
    {
        if(bit_is_set(PINC, 7)) 
        {
            set(PORTD,1); // if there is input in PC7, set the output pin high
        }
        else
        {
            clear(PORTD,1);// if there is no input in PC7, clear the output pin low
        }
    }
    return 0; /* never reach*/
}