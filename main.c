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
 * 
 * TIMING
 * 
 * From controller datasheet at 1/64 duty a 70Hz frame rate requires a 250kHz
 * clock or a cycle time of <4us
 * 
 * Elsewhere the busy time is given as between 1/Fclk and 3/Fclk, making the
 * maximum busy time 12us, using a E high and low duration of 8us guarantees 
 * this
 * 
 * Since it takes only 1us to execute a read or write there is some gain from 
 * busy polling
 * 
 * On displays tested so far it appears that a status read is not latched 
 * meaning that it is possible to set RS=0,R/W=1,E=1 and wait for D7 to go low 
 * though this is so far undocumented and some controllers might require 
 * cycling of E
 */
/* 
 * glcd control pins
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
#define glcdset(X) glcdcont|=(X)
#define glcdunset(X) glcdcont&=~(X)
/*
 *  Control:
 */
#define glcdc_off 0b00111110
#define glcdc_on 0b00111111
#define glcdc_y  0b01000000
#define glcdc_x  0b10111000
 //*  11XXXXXX=Set start line "Z"/Vertical scroll

void glcd_clear(void)
{
    glcdunset(glcd_rs);
    glcddata=glcdc_y;
    glcdunset(glcd_cs1);
    glcdset(glcd_cs2);
    glcdset(glcd_e);
    __delay_us(8);
    glcdunset(glcd_e);
    __delay_us(8);
    glcdset(glcd_cs1);
    glcdunset(glcd_cs2);
    glcdset(glcd_e);
    __delay_us(8);
    glcdunset(glcd_e);
    __delay_us(8);
    for (uint8_t xx=0b10111000; xx<=0b10111111;xx++)
    {
        glcddata=xx;
        glcdunset(glcd_cs1);
        glcdset(glcd_cs2);
        glcdset(glcd_e);
        __delay_us(8);
        glcdunset(glcd_e);
        __delay_us(8);
        glcdset(glcd_cs1);
        glcdunset(glcd_cs2);
        glcdset(glcd_e);
        __delay_us(8);
        glcdunset(glcd_e);
        __delay_us(8);
        glcddata=0;
        glcdset(glcd_rs);
        for(uint8_t aa =0; aa<=0x3f; aa++)
        {
            glcdunset(glcd_cs1);
            glcdset(glcd_cs2);
            glcdset(glcd_e);
            __delay_us(8);
            glcdunset(glcd_e);
            __delay_us(8);
            glcdset(glcd_cs1);
            glcdunset(glcd_cs2);
            glcdset(glcd_e);
            __delay_us(8);
            glcdunset(glcd_e);
            __delay_us(8);
        }
        glcdunset(glcd_rs);
        
    }
}
void glcd_setpixel(uint8_t x,uint8_t y,uint8_t c)
{
    uint8_t d;
    if (x>0x7f || y>0x3f) return;
    if (x&0x40) {
        glcdset(glcd_cs1);
        glcdunset(glcd_cs2);
    } else {
        glcdunset(glcd_cs1);
        glcdset(glcd_cs2);
    }
    glcdunset(glcd_rs);
    glcddata=glcdc_x|((y>>3)&7);
    glcdset(glcd_e);
    __delay_us(8);
    glcdunset(glcd_e);
    __delay_us(8);
    glcddata=glcdc_y|(x&0x3f);
    glcdset(glcd_e);
    __delay_us(8);
    glcdunset(glcd_e);
    __delay_us(8);
    //read in
    glcdtris=0xff;
    glcdset(glcd_rw);
    glcdset(glcd_rs);
    glcdset(glcd_e);
    __delay_us(8);
    glcdunset(glcd_e);
    __delay_us(8);
    glcdset(glcd_e);
    __delay_us(8);
    d=glcddata;
    glcdunset(glcd_e);
    __delay_us(8);
    //write out
    glcdunset(glcd_rw);
    glcdunset(glcd_rs);
    glcdtris=0x00;
    glcddata=glcdc_y|(x&0x3f);
    glcdset(glcd_e);
    __delay_us(8);
    glcdunset(glcd_e);
    __delay_us(8);
    if (c)
    {
        d|=1<<(y&7);
    } else {
        d&=~(1<<(y&7));
    }
    glcdset(glcd_rs);
    glcddata=d;    
    glcdset(glcd_e);
    __delay_us(8);
    glcdunset(glcd_e);
    __delay_us(8);
    glcdunset(glcd_rs);
}
void main(void) {
    ANSELD=0;
    /*
     * Initialise glcd
     */
    glcdcont_tris=0b11000000;
    glcdcont=glcd_ini;
    __delay_ms(100);
    glcdset(glcd_rst);
    glcdtris=0x00;
    glcddata=glcdc_on;
    glcdunset(glcd_cs1);
    glcdset(glcd_e);
    __delay_us(8);
    glcdunset(glcd_e);
    __delay_us(8);
    glcdset(glcd_cs1);
    glcdunset(glcd_cs2);
    glcdset(glcd_e);
    __delay_us(8);
    glcdunset(glcd_e);
    __delay_us(8);
    glcd_clear();
    for(uint8_t x=0;x<64;x++)
        glcd_setpixel(x,x,1);
    
    for(uint8_t x=10;x<=100;x++)
    {
        glcd_setpixel(x,10,1);
        glcd_setpixel(x,50,1);
    }
    for(uint8_t y=11;y<50;y++)
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
        __delay_us(8);
        glcdunset(glcd_e);
        __delay_us(8);
    }
  */  
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
