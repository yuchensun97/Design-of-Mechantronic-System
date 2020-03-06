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
    _delay_ms(10); // dummy routine to pretend that we have other things to do
}

unsigned int oldtime, tperiod;

int hightime()
{
    while(!bit_is_set(TIFR3, ICF3)); //check whether there is an input
    set(TIFR3, ICF3); // clear flag by writing 1 into the register
    tperiod = ICR3 - oldtime; // calculate the counts between each falling edge
    oldtime = ICR3; // set old time equals to ICR3
    return tperiod; // return counts between each falling edge
}

int main(void)
{
    m_usb_init(); //initialize USB for print statement
    set(DDRD, 1); // set PD1 as output (connect to grenn LED)
    set(DDRD, 2); // set PD2 as output (connect to red LED)
    clear(DDRC, 7); //clear value in pin C7
    clear(PORTD, 1); //clear output value in pin D1
    clear(PORTD, 2); //clear output value in pin D2
    teensy_clockdivide(0); // set the default clock speed to 16MHz
    set(TCCR3B, CS32); set(TCCR3B, CS30);// set timer prescaler to /1024
    float t; // create a variable of IR period
    /*insert your initialization here*/

    while(!bit_is_set(TIFR3, ICF3))     dosomething(); // to check the input
    if(bit_is_set(TIFR3, ICF3)) // if there is an input, set oldtime equals to ICR3
    {
        oldtime = ICR3;
    }

    while(1) // the infinite loop
    {
        t = hightime(); // get the value of IR period
        if ((t<41)&(t>31)) // when there is a 435Hz IR
        {
            set(PORTD,1); // light up LED 1
            clear(PORTD,2); // turn off LED 2
        }
        if((t<97)&(t>87)) //when there is a 170Hz IR
        {
            set(PORTD,2); // light up LED 2
            clear(PORTD,1); //turn off LED 1
        }
    }

    return 0;
}