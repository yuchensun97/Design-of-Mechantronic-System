/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h"

#define PRINTNUM(x) m_usb_tx_uint(x); m_usb_tx_char(10); m_usb_tx_char(13)

void dosomething()
{
    _delay_ms(100); // dummy routine to pretend that we have other things to do
}

unsigned int oldtime, tperiod, tperiod_t;

int presstime()
{   
    while(!bit_is_set(TIFR3, ICF3))     dosomething(); // check input capture flag
    set(TIFR3, ICF3); // writing 1 to the register to clear the overflow flag
    tperiod = ICR3 - oldtime; //measure the period of 2 hits
    oldtime =ICR3; // set oldtime equals to ICR3
    tperiod_t = tperiod/ 15.625; // switch counts into ms
    return tperiod_t; // return the time value between 2 hits
}

int main()
{
    m_usb_init(); //initialize USB for print statement
    teensy_clockdivide(0); // set the timer to 16MHz 
    set(TCCR3B,CS32); // Start timer with /256 prescaler
    int t[4]; // create a set of the time to hit press
    int avg; // average time for hit press
    int i; // create the times of hitting press
    int sum = 0; // sum of elements in vector t
    /*insert your initialization here*/

    while(1) /* main loop*/
    {   
        while(!bit_is_set(TIFR3, ICF3))     dosomething(); // wait for the switch to be pressed
        set(TIFR3, ICF3); // writing 1 to the register to clear the overflow flag
        m_usb_tx_string("press to start"); // after pressing the switch, "press to start should appear on the screen"
        while(!bit_is_set(TIFR3, ICF3))     dosomething(); // wait for the switch to be pressed
        set(TIFR3, ICF3); // writing 1 to the register to clear the overflow flag
        oldtime = TCNT3; // set oldtime equals to the absolute value in timer 3.

        for(i=0; i < 4; i++) //There are 4 intervals in a 5-press game      
	{   
            t[i] = presstime(); // store the time value between 2 hits in t
            m_usb_tx_string("Time between last press");
            sum = sum + t[i]; // calculate the total time of pressing 
            PRINTNUM(t[i]); // print each time between 2 hits
        }
        avg = sum/4; // calculate the average value
        sum = 0; // set the total time to 0 for next loop
        m_usb_tx_string("The avarage time is"); 
        PRINTNUM(avg); // print the average value
    }
}  