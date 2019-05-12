/*
 * File:   main.c
 * Author: Oliver
 *
 * Created on 16 December 2018, 13:17
 */


#include <xc.h>
#include <stdint.h>

#include "glcd_lib.h"

/* GLCD details for EasyPIC
 * 
 *  1	CS1	RB0	Active low
 *  2	CS2	RB1	Active low
 *  3	GND
 *  4	VCC
 *  5	Vo
 *  6	RS	RB2	0=Control
 *  7	R/W	RB3	0=Write
 *  8	E	RB4	Active High
 *  9	D0	RD0
 *  10	D1	RD1
 *  11	D2	RD2
 *  12	D3	RD3
 *  13	D4	RD4
 *  14	D5	RD5
 *  15	D6	RD6
 *  16	D7	RD7
 *  17	RST	RB5	Active Low
 *  18	VEE	
 *  19	LED+
 *  20	LED-
 */
void main(void) {
    ANSELD=0;
    //OSCCON NEEDED FOR PIC18F45K22
#ifdef OSCCON
    OSCCON=0X72;
#endif
    /*
     * Initialize glcd
     */
    glcd_reset();
    __delay_ms(100);
    glcd_on();
    glcd_clear();
    for(uint8_t x=0;x<64;x++)
        glcd_vline(x+32,x,63-x,1);
    for(uint8_t x=4;x<28;x++)
        glcd_vline(x+32,x+2,61-x,0);
    
    for(uint8_t x=10;x<=100;x++)
    {
        glcd_setpixel(x,15,1);
        glcd_setpixel(x,50,1);
    }
    for(uint8_t y=16;y<50;y++)
    {
        glcd_setpixel(10,y,1);
        glcd_setpixel(100,y,1);
    }
/*    glcdunset(glcd_cs1);
    glcdset(glcd_cs2);
    glcdset(glcd_rs);
    for(uint8_t aa =0; aa<=0x3f; aa++)
    {
        glcddata=aa;
        glcdset(glcd_e);
        glcd_wait();
        glcdunset(glcd_e);
        glcd_wait();
    }
  */  
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    glcd_clear();
    for(uint8_t aa=10;aa<=100;aa+=10)
    {
        glcd_line(aa,10,10,53,1);
    }
    //glcd_line(73,10,54,53,1);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    glcd_clear();
    glcd_fill_circle(64,32,30,1);
    TRISD=0x00;
    while (1)
    {
        PORTD=0x55;
        __delay_ms(200);
        PORTD=~PORTD;
        __delay_ms(200);
    }
    
    return;
}
