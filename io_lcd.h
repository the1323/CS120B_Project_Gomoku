#ifndef IO_LCD_H
#define IO_LCD_H

#include<stdio.h>
#include<stdbool.h>
#include<inttypes.h>
#include<avr/io.h>
#include<util/delay.h>
#include<avr/pgmspace.h>
#include<limits.h>


#define LCD_PORT PORTB
#define LCD_DDR DDRB
#define LCD_PIN PINB
#define dc 1
#define cs 4
#define rst 0



#define BLACK 0x0000
#define WHITE 0xFFFF


void LCD_Setup(void);
void LCD_WriteCommand(uint8_t com);
void LCD_WriteData8(uint8_t data);
void LCD_WriteData16(uint16_t data);
void LCD_init(void);
void LCD_ClearScreen();
void LCD_DrawPixel(uint16_t x3,uint16_t y3,uint16_t colour1);
void LCD_DrawBlock(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour);
void LCD_WriteChar( unsigned char c);
void LCD_WriteString(unsigned char* c);
void LCD_WriteInt(int i);
void LCD_Cursor(uint16_t x,uint16_t y);
void delay_ms(int t);
/////////////////////////////////////////////////////////////////
void LCD_DrawBoard();
void LCD_ShowPlayer(unsigned char i);
void LCD_ShowBattery(unsigned short i);
void LCD_ShowScore(unsigned short b,unsigned short w);
void LCD_GameCursor(unsigned char x,unsigned char y);

void LCDGAMEINTI();
#endif // IO_LCD_H
