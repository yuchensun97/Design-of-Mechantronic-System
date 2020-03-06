/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h"

#define PRINTNUM(x) m_usb_tx_uint(x); m_usb_tx_char(10); m_usb_tx_char(13)

#define DIDR0 _SFR_MEM8(0x7E)
#define DIDR _SFR_MEM16(0x7D)

int chanel, read, toRtn; //create a value to store ADC input channel

void adc_setup(unsigned int ADCchannel) // subroutine for ADC set up
{
    unsigned char tmp; // create a value to store the bits of DIDR
    tmp = 1<< ADCchannel; // left shift ADC channel once
    DIDR0 |= tmp; // or with tmp
    DIDR2 |= tmp>>8; // or with tmp after right shift 8 times
    m_usb_tx_string("\rADCinput =");
    PRINTNUM(ADCchannel); 
}

void adc_read(unsigned int RC) // subroutine for reading ADC
{
    clear(ADMUX, MUX2); // clear bit for ADMUX
    clear(ADMUX, MUX1); // clear bit for ADMUX
    clear(ADMUX, MUX0); // clear bit for ADMUX

    if (RC<8) // if the set-up channel is less than 8
    {
        ADMUX |= RC; // or with RC
        clear(ADCSRB, MUX5); // when RC is less than 8, the MUX5 in ADCSRB is 0
        set(ADMUX,REFS0); // the reference voltage of ADC is Vcc
    }
    else
    {
        ADMUX |= RC-8; // or with RC
        set(ADCSRB,MUX5); // when RC is more than 8, the MUX 5 in ADCSRB is 1
        set(ADMUX,REFS0); // the reference voltage of ADC is Vcc
    }
    
    set(ADCSRA, ADEN); // enable ADC
    set(ADCSRA, ADSC); // start conversation
    while(bit_is_set(ADCSRA,ADSC)); // wait for conversation to finish
    toRtn = ADC; // store reading values in toRtn
    clear(ADCSRA, ADSC);// clear the conversation complete flag
    m_usb_tx_string("\rADCReading ="); 
    PRINTNUM(toRtn); // print the reading angle
}

int main(void)
{
    m_usb_init(); // initialize USB communication
    
    set(ADMUX,REFS0); // set the reference voltage to Vcc
    teensy_clockdivide(0); //set defalut clock of teensy to 16MHz
    set(ADCSRA, ADPS2); set(ADCSRA, ADPS1); set(ADCSRA, ADPS0); // set the prescaler to /128
    while(1)
    {
        adc_setup(1); // set up ADC 1
        adc_read(1); // read ADC 1
        adc_setup(11); // set up ADC 11
        adc_read(11); // read ADC 11
        adc_setup(12); // set up ADC 12
        adc_read(12); // read ADC 12
    }
    return 0;
}