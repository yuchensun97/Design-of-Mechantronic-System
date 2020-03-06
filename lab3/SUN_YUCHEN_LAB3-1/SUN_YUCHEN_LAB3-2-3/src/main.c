/* Name: main.c
 * Author: Yuchen Sun
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h"

#define PRINTNUM(x) m_usb_tx_uint(x); m_usb_tx_char(10); m_usb_tx_char(13)
#define COMPAREVALUE 0x00FF

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

int adc_read(unsigned int RC) // subroutine for reading ADC
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
    toRtn = ADC/3.41; // store reading angle in toRtn
    clear(ADCSRA, ADSC);// clear the conversation complete flag
    m_usb_tx_string("\rADCReading ="); 
    return toRtn; // print the reading angle
}

int main(void)
{
    m_usb_init(); // initialize USB communication
    
    set(ADMUX,REFS0); // set the reference voltage to Vcc
    teensy_clockdivide(0); //set defalut clock of teensy to 16MHz
    set(ADCSRA, ADPS2); set(ADCSRA, ADPS1); set(ADCSRA, ADPS0); // set the prescaler to /128
    set(DDRB, 5); // set PB5 as output
    set(DDRB, 6); // set PB6 as output
    set(DDRB, 7); // set PB7 as output
    set(TCCR1B,CS12); set(TCCR1B,CS10); // set the prescaler to /1024
    set(TCCR1B,WGM12); set(TCCR1A,WGM10); // set timer 1 to mode 5
    int angle_1, angle_2, angle_3; // potentiometer's angle
    adc_setup(1);
    adc_setup(4);
    adc_setup(5);
    while(1)
    {
        
        angle_1 = adc_read(1); // get the right angle of angle 1
        angle_2 = 180-adc_read(4); // get the right angle of angle 2
        angle_3 = 180-adc_read(5); // get the right angle of angle 3
        //set(PORTC,6);
        set(TCCR1A,COM1A1); // clear at OCR1A, set at rollover
        set(TCCR1A, COM1B1);// clear at OCR1B, set at rollover
        set(TCCR1A,COM1C1);// clear at OCR1C, set at rollover
        //duty_circle = (angle+46)/180/8;
        if (angle_1<180) // if angle 1 is right
        {
            OCR1A = 0.65*(COMPAREVALUE*(angle_1))/180/4; // set the OCR1A to drive the motor
        }
        else
        {
            teensy_wait(100); // if the angle is beyond motor's limit,wait
            
        }
        if (angle_2<180 & angle_2>0) // if angle 2 is right
        {
            OCR1B = 0.75*(COMPAREVALUE*(angle_2))/180/4; // set the OCR1B to drive the motor
        }
        else
        {
            teensy_wait(100); // if the angle is beyond motor's limit, wait
        }
        if (angle_3<180 & angle_3>0) // if angle 3 is right
        {
            OCR1C = 0.75*(COMPAREVALUE*(angle_3))/180/4; // set the OCR1C to drive the motor
        }
        else
        {
            teensy_wait(100); // if the angle is beyond motor's limit, wait.
        }
    }
    return 0;
}