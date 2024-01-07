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
 * Uncomment if using MCC to insert MCC.h _XTAL_FREQ definition
 */
//MCC Classic
//#include "mcc_generated_files/mcc.h"
//MCC Melody
//#include "mcc_generated_files/system/system.h"
/* 
 * glcd control pins
 */
#define glcdcont_lat LATB
#define glcdcont_tris TRISB
#define glcddata_port PORTD
#define glcddata_tris TRISD
#define glcd_cs1 0b00000001
#define glcd_cs2 0b00000010
#define glcd_rs  0b00000100
#define glcd_rw  0b00001000
#define glcd_e   0b00010000
#define glcd_rst 0b00100000
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

