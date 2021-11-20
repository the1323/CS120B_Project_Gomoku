#ifndef IO_LCD_H
#define IO_LCD_H

#include<avr/io.h>
#include<util/delay.h>
#include<avr/pgmspace.h>
#include<limits.h>
#include<inttypes.h>

/* Author: The033, CS120B UCR final project.
 * This driver library is designed for ILI9341 Res 280x320 TFT LCD for Atmega1284/p.
 * This display has same interface as 5110 for all hardware pin arragnement
 * and operating voltage.
 *
 * There are 8 output pins required for display.
 *
 * Here are the pinout
 * ILI9341   Atmega1284/p
 * VCC       3.3V(+)
 * GND       GND(-)
 * CS        PB4
 * RESET     PB0
 * D/C       PB1
 * MOSI      PB5
 * SCK       PB7
 * LED       3.3V(+)
 *

*/

/*
 * typedef according to POSIX:
 * uint8_t  -> unsigned int 8 bits
 * uint16_t -> unsigned int 16 bits
 */
void LCD_setup(void);
void LCD_WriteCommand(uint8_t com);
void LCD_WriteData(uint8_t data);
void LCD_init(void);
void LCD_pushcolour(uint16_t colour);
void LCD_ClearScreen(uint16_t colour);
//void LCD_drawpixel(uint16_t x3,uint16_t y3,uint16_t colour1);
void delay_ms(int miliSec);
#endif // IO_LCD_H
