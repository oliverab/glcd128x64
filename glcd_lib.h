/*
 * File:   glcd_lib.h
 * Author: Oliver
 *
 * Created on 12 May 2019, 21:10
 */

#ifndef GLCD_LIB_H
#define	GLCD_LIB_H

#include <xc.h>  
#include <stdint.h>

void glcd_reset(void);
void glcd_on(void);
void glcd_clear(void);
void glcd_setpixel(uint8_t x,uint8_t y,uint8_t c);
void glcd_vline(uint8_t x,uint8_t y1,uint8_t y2,uint8_t c);
void glcd_line(uint8_t x1,uint8_t y1, uint8_t x2,uint8_t y2,uint8_t c);
void glcd_circle(uint8_t x,uint8_t y,uint8_t r,uint8_t c);
void glcd_fill_circle(uint8_t x,uint8_t y,uint8_t r,uint8_t c);
void glcd_fill_donut(uint8_t x,uint8_t y,uint8_t r1,uint8_t r2,uint8_t c);
void glcd_systext(uint8_t x,uint8_t y,const char *tx,uint8_t c);
void glcd_set_start_line(uint8_t l);

#endif	/* GLCD_LIB_H */

