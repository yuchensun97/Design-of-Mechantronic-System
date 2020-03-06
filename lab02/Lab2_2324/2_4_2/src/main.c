/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h"

#define PRINTNUM(x) m_usb_tx_uint(x); m_usb_tx_char(10); m_usb_tx_char(13)

unsigned int rising_time, falling_time, last_falling; // create a variable to store pulse width

int pulse_width()
{
    int width; // create a variable to store the pulse width
    set(TCCR3B, ICES3); // store time value on rising edge
    while(!bit_is_set(TIFR3, ICF3)); // check input capture
    while(bit_is_set(TIFR3, ICF3))
    {
        rising_time = ICR3; // store ICR3 value into rising time.
        set(TIFR3, ICF3); // writing 1 to the register to clear the overflow flag
        clear(TCCR3B, ICES3); // store time value on falling edge
        while(!bit_is_set(TIFR3, ICF3)); // wait for falling edge
        break; // when a falling edge is detected, quit the loop
    }  
    while(bit_is_set(TIFR3, ICF3))
    {
        falling_time = ICR3; // store ICR3 value into falling time.
        set(TIFR3, ICF3); // writing 1 to the register to clear the overflow flag
        break; // quit the loop
    }
    width = 4*(falling_time - rising_time); // calculate pulse width.
    return width; //return the width of each pulse
}

unsigned int tperiod;

int interval() //create a subroutine that calculate time between sync and sweep pulse
{
    set(TIFR3, ICF3); // clear the overflow flag by writing 1 into the register
    tperiod = ICR3 - last_falling; // calculate counts between sync and sweep pulse
    return tperiod*4; // return the time value(us) of time between sync and swwep pulse
}

int main(void)
{
    m_usb_init(); //initialize USB for print statement
    clear(DDRC, 7); //clear value in pin C7
    teensy_clockdivide(0); // set the default clock speed to 16MHz
    set(TCCR3B, CS30); set(TCCR3B, CS31);// set timer prescaler to /64
    int width; // create a variable to store the value of pulse width
    int sync = 0; // create a variable to store the postion of sync pulse
    int x_flag = 0; //create a flag to mark each x pulse
    int y_flag = 0; //create a flag to mark each y pulse
    int x = 0; // interval between x and sync
    int y = 0; // interval between y and sync
    /*insert your initialization here*/

    while(1)
    {
        if(sync>10){sync=0;} //if there are continuous sync pulse, clear the sync value
        width = pulse_width(); // set width to pulse width
        if (width > 70) // if there is sync pulse
        {
            sync +=1; // mark the number of sync pulse
            last_falling = falling_time; // store the absolute time value on falling edge
        }
        if ((width <20) & (width > 3)) //if there is a sweep pulse
        {
            if (sync == 3) // if there is an x pulse
            {
                x_flag = 1; // writing 1 to x flag
                x = interval(); // get the time value between x the sync pulse
                m_usb_tx_string("x"); //to check whether x pulse is recieved.
                sync = 0; // set sync to 0
            }
            
            else if(sync==1) // if there is a y pulse
            {
                y_flag = 1; // writing 1 to y flag
                y = interval(); // get the time value between y and sync pulse
                m_usb_tx_string("y"); // to check whether y pulse is recieved.
                sync = 0; // set sync to 0
            }
        }
        PRINTNUM(sync); // print the step of sync pulse
        if ((x_flag == 1) & (y_flag == 1)) // only when both x and y pulses are recieved, then...
        {
            m_usb_tx_string("interval between x and sync");
            PRINTNUM(x); // print the time value of x pulse
            m_usb_tx_string("interval between y and sync");
            PRINTNUM(y); //print the time value of y pulse
            x_flag = 0; y_flag = 0; // clear flag
        }
    }
    return 0; /*never reach*/
}