/*
 * File:   main.c
 * Author: Oliver
 *
 * Created on 16 December 2018, 13:17
 */


#include <xc.h>
#include <stdint.h>

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
 *  
 *  Control:
 *  0011111X=Display 0=off,1=on
 *  01XXXXXX=Set "Y" address (horizontal)
 *  10111XXX=Set "X" page (vertical)
 *  11XXXXXX=Set start line "Z"/Vertical scroll
 * 
 * Bit organisation LSB at top, MSB bottom
 * 
 *  Status:
 *  7=Busy
 *  6=0
 *  5=on/off (1=off)
 *  4=Reset (1=Reset)
 *  3-0=0
 */
/* 
 * glcd conrol pins
 */
#define glcdcont LATB
#define glcdcont_tris TRISB
#define glcd_cs1 0b00000001
#define glcd_cs2 0b00000010
#define glcd_rs  0b00000100
#define glcd_rw  0b00001000
#define glcd_e   0b00010000
#define glcd_rst 0b00100000
#define glcd_ini (glcd_cs1|glcd_cs2)
#define glcddata PORTD
#define glcdtris TRISD
/*
 *  Control:
 */
#define glcdc_off 0b00111110
#define glcdc_on 0b00111111
#define glcdc_y  0b01000000
 //*  10111XXX=Set "X" page (vertical)
 //*  11XXXXXX=Set start line "Z"/Vertical scroll

void main(void) {
    /*
    */
    glcdcont_tris=0b11000000;
    glcdcont=glcd_ini;
    __delay_ms(100);
    glcdcont|=glcd_rst;
    glcdtris=0x00;
    glcddata=glcdc_on;
    glcdcont&=~glcd_cs1;
    glcdcont|=glcd_e;
    __delay_us(4);
    glcdcont&=~glcd_e;
    __delay_us(4);
    glcdcont|=glcd_cs1;
    glcdcont&=~glcd_cs2;
    glcdcont|=glcd_e;
    __delay_us(4);
    glcdcont&=~glcd_e;
    __delay_us(4);
    glcdcont&=~glcd_cs1;
    glcdcont|=glcd_cs2;
    glcdcont|=glcd_rs;
    for(int aa =0; aa<=0xff; aa++)
    {
        glcddata=aa;
        glcdcont|=glcd_e;
        __delay_us(4);
        glcdcont&=~glcd_e;
        __delay_us(4);
    }
    
    TRISD=0x00;
    while (1)
    {
        PORTD=0x55;
        __delay_ms(200);
        PORTD=0xAA;
        __delay_ms(200);
    }
    
    return;
}
