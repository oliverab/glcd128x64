/*
 * File:   glcd_hardware_sample.h
 * Author: Oliver Broad
 *
 * Created on 13 May 2019, 20:53
 * 
 * This is a TEMPLATE for the hardware configuration, to be copied to
 * the main project, renamed to glcd_hardware.h, then customised to reflect
 * the specific system
 * 
 * It is intended that read/write macros may be redefined to allow use of an
 * I/O expander
 * 
 */
#ifndef GLCD_HARDWARE_H
#define	GLCD_HARDWARE_H

#include <xc.h>
/* 
 * glcd control pins
 */
#define glcdcont_lat LATB
#define glcdcont_tris TRISB
#define glcddata_port PORTD
#define glcddata_tris TRISD
/*
 * Macros to access display pins: can be replaced with function calls to
 * allow use of an I/O expander
 */
#define glcdcont_write(X) glcdcont_lat=(X)
#define glcdcont_wr_tris(X) glcdcont_tris=(X)
#define glcdcont_set(X) glcdcont_lat|=(X)
#define glcdcont_unset(X) glcdcont_lat&=~(X)
#define glcddata_write(X) glcddata_port=(X)
#define glcddata_wr_tris(X) glcddata_tris=(X)
#define glcddata_read() glcddata_port
#define glcd_wait() __delay_us(8);


#endif	/* GLCD_HARDWARE_H */

