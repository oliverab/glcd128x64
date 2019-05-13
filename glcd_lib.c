/*
 * File:   glcd_lib.c
 * Author: Oliver
 *
 * Created on 12 May 2019, 20:55
 */


#include <xc.h>
#include <stdint.h>

#include "glcd_lib.h"


/*  Glcd control signals and commands
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
#include "glcd_hardware.h"
#define glcd_cs1 0b00000001
#define glcd_cs2 0b00000010
#define glcd_rs  0b00000100
#define glcd_rw  0b00001000
#define glcd_e   0b00010000
#define glcd_rst 0b00100000
#define glcd_ini (glcd_cs1|glcd_cs2)
/*
 *  Control:
 */
#define glcdc_off 0b00111110
#define glcdc_on 0b00111111
#define glcdc_y  0b01000000
#define glcdc_x  0b10111000
 //*  11XXXXXX=Set start line "Z"/Vertical scroll

void glcd_reset(void)
{
    glcdcont_wr_tris(0b11000000);
    glcdcont_write(glcd_ini);
}
void glcd_on(void)
{
        glcdcont_set(glcd_rst);
    glcddata_wr_tris(0x00);
    glcddata_write(glcdc_on);
    glcdcont_unset(glcd_cs1);
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();
    glcdcont_set(glcd_cs1);
    glcdcont_unset(glcd_cs2);
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();

}

void glcd_clear(void)
{
    glcdcont_unset(glcd_rs);
    glcddata_write(glcdc_y);
    glcdcont_unset(glcd_cs1);
    glcdcont_set(glcd_cs2);
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();
    glcdcont_set(glcd_cs1);
    glcdcont_unset(glcd_cs2);
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();
    for (uint8_t xx=0b10111000; xx<=0b10111111;xx++)
    {
        glcddata_write(xx);
        glcdcont_unset(glcd_cs1);
        glcdcont_set(glcd_cs2);
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();
        glcdcont_set(glcd_cs1);
        glcdcont_unset(glcd_cs2);
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();
        glcddata_write(0);
        glcdcont_set(glcd_rs);
        for(uint8_t aa =0; aa<=0x3f; aa++)
        {
            glcdcont_unset(glcd_cs1);
            glcdcont_set(glcd_cs2);
            glcdcont_set(glcd_e);
            glcd_wait();
            glcdcont_unset(glcd_e);
            glcd_wait();
            glcdcont_set(glcd_cs1);
            glcdcont_unset(glcd_cs2);
            glcdcont_set(glcd_e);
            glcd_wait();
            glcdcont_unset(glcd_e);
            glcd_wait();
        }
        glcdcont_unset(glcd_rs);
        
    }
}
void glcd_setpixel(uint8_t x,uint8_t y,uint8_t c)
{
    uint8_t d;
    if (x>0x7f || y>0x3f) return;
    if (x&0x40) {
        glcdcont_set(glcd_cs1);
        glcdcont_unset(glcd_cs2);
    } else {
        glcdcont_unset(glcd_cs1);
        glcdcont_set(glcd_cs2);
    }
    glcdcont_unset(glcd_rs);
    glcddata_write((uint8_t)(glcdc_x|((y>>3)&7)));
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();
    glcddata_write((uint8_t)(glcdc_y|(x&0x3f)));
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();
    //read in
    glcddata_wr_tris(0xff);
    glcdcont_set(glcd_rw);
    glcdcont_set(glcd_rs);
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();
    glcdcont_set(glcd_e);
    glcd_wait();
    d=glcddata_read();
    glcdcont_unset(glcd_e);
    glcd_wait();
    //write out
    glcdcont_unset(glcd_rw);
    glcdcont_unset(glcd_rs);
    glcddata_wr_tris(0x00);
    glcddata_write((uint8_t)(glcdc_y|(x&0x3f)));
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();
    if (c)
    {
        d|=1<<(y&7);
    } else {
        d&=~(1<<(y&7));
    }
    glcdcont_set(glcd_rs);
    glcddata_write(d);    
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_rs);
}
void glcd_vline(uint8_t x,uint8_t y1,uint8_t y2,uint8_t c)
{
    uint8_t y;
    if (y1>y2) {uint8_t t=y2;y2=y1;y1=t;}
    if (x>0x7f || y1>0x3f) return;
    if (x&0x40) {
        glcdcont_set(glcd_cs1);
        glcdcont_unset(glcd_cs2);
    } else {
        glcdcont_unset(glcd_cs1);
        glcdcont_set(glcd_cs2);
    }
    glcdcont_unset(glcd_rs);
    for(y=(y1 & 0xf8); y <= (y2&0xf8);y+=8)
    {
        uint8_t m=0;
        uint8_t d;
        if (y==(y1&0xf8)) // match line start
        {
            m|=((1<<(y1&7))-1);
        }
        if (y==(y2&0xf8)) // match line end
        {
            m|=(0xfe<<(y2&7));
        }
        glcddata_write(glcdc_x | ((y >> 3)&7));
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();
        if (m) 
        {
            glcddata_write(glcdc_y | (x & 0x3f));
            glcdcont_set(glcd_e);
            glcd_wait();
            glcdcont_unset(glcd_e);
            glcd_wait();
            //read in
            glcddata_wr_tris(0xff);
            glcdcont_set(glcd_rw);
            glcdcont_set(glcd_rs);
            glcdcont_set(glcd_e);
            glcd_wait();
            glcdcont_unset(glcd_e);
            glcd_wait();
            glcdcont_set(glcd_e);
            glcd_wait();
            d = glcddata_read();
            glcdcont_unset(glcd_e);
            glcd_wait();
            //write out
            glcdcont_unset(glcd_rw);
            glcdcont_unset(glcd_rs);
            glcddata_wr_tris(0x00);
            
        }
        glcddata_write(glcdc_y | (x & 0x3f));
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();
        if (c) 
        {
            d |= ~m;
        } else {
            d &= m;
        }
        glcdcont_set(glcd_rs);
        glcddata_write(d);
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_rs);
    }
}
void glcd_line(uint8_t x1,uint8_t y1, uint8_t x2,uint8_t y2,uint8_t c)
/*
 * Modified Bresenham line draw, extended to draw steep lines as a series of
 * vertical lines not dots due to display memory organisation
 *
 */
{
    int8_t dx=x2-x1;
    if (dx<0) dx=-dx;
    dx++;
    int8_t dy=y2-y1;
    if (dy<0) dy=-dy;
    dy++;
    uint8_t d=dx/2;
    int d2;
    while (x1!=x2)
    {
        d+=dy;
        if (d>=dx) 
        {
            d2=d / dx;
            if (y2>y1)
            {
                glcd_vline(x1,y1,y1+d2-1,c);
                y1+=d2;
            } else {
                glcd_vline(x1,y1,y1-d2+1,c);
                y1-=d2;
            }
            d%=dx;
/*              
            while (d>=dx)
            {
                glcd_setpixel(x1,y1,c);
                d-=dx;
                if (y2>y1) y1++; else y1--;
            }
*/            
        }
        else
        {
            glcd_setpixel(x1,y1,c);
        }
        if (x2>x1) x1++; else x1--;
    }
    glcd_vline(x1,y1,y2,c);
}
void glcd_fill_circle(uint8_t x,uint8_t y,uint8_t r,uint8_t c)
{
    int8_t d;
    uint8_t x1;
    uint8_t y1;
    d=0;
    x1=0;
    y1=r;
    while (x1<=r){
        glcd_vline(x+x1,y-y1,y+y1,c);
        if (x1) glcd_vline(x-x1,y-y1,y+y1,c);
        d+=2*x1+1;
        x1++;
        while (d>0 && y1>0)
        {
            y1--;
            d-=2*y1+1;
        }
    }
}
