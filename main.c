/*
 * File:   main.c
 * Author: Oliver
 *
 * Created on 16 December 2018, 13:17
 */


#include <xc.h>

void main(void) {
    TRISB=0x00;
    while (1)
    {
        PORTB=0x55;
        __delay_ms(200);
        PORTB=0xAA;
        __delay_ms(200);
    }
    return;
}
