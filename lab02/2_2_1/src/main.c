/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h"

int main(void)
{
    set(DDRD, 2); // set PD2 as output
    clear(DDRC, 7); //clear the value in input pin
    clear(PORTD, 2); // Take the output pin PD2 low

    for(;;)
    {
        if(!bit_is_set(PINC, 7)) 
        {
            set(PORTD,2); // if there is no input in PC7, set the output pin
        }
        else
        {
            clear(PORTD,2);// if there is input in Pc7, clear the output pin
        }
    }
    return 0;
}