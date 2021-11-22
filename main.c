#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>
#include<stdbool.h>
#include<inttypes.h>
#include "io_lcd.h"



void ADC_init(void)
{
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}
uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADC;
}
int main(void)
{
	char joy []="w";
	DDRA = 0x00; PORTA = 0xFF; //joystick
	ADC_init();
	unsigned short analog_X;
	unsigned short analog_Y;
	
	
	
	LCD_init();
	_delay_ms(200);
	LCD_ClearScreen();
		
	
	_delay_ms(5000);
	int cnt =0;
	char a[] = "a";

	a[0]+=cnt;
	
	LCD_Cursor(4,4);

	_delay_ms(2);

	LCD_WriteString("Black Turn");
	LCD_Cursor(190,4);

	LCD_WriteString("100");
	for(int y =20;y<260;y+=20){
		for (int x =0;x<240;x++)
		{LCD_DrawPixel(4+x,y+4,BLACK);
		}
	}
	for(int y =20;y<240;y++){
		for (int x =0;x<240;x+=20)
		{LCD_DrawPixel(0+x,y+4,BLACK);
		}
	}
	LCD_Cursor(4,24);
	LCD_WriteChar('o');
	LCD_Cursor(4,260);
	LCD_WriteString("Time Left ");
	LCD_Cursor(4,280);

	LCD_WriteString("Black Score 123");

	LCD_Cursor(4,300);
	LCD_WriteString("joystick");
	char snum[5];
	unsigned short t = 59;

	while(1)
	{
		itoa(t, snum, 10);
		LCD_Cursor(120,260);
		LCD_WriteString(snum);
		t--;
		analog_X = ReadADC(0);
		analog_Y = ReadADC(1);
		_delay_ms(100);
		LCD_Cursor(120,300);

		itoa(t, snum, 10);
		LCD_WriteString(joy);
		
		if (analog_X >= 900) { joy[0] = 'R'; }
		else if (analog_X <= 200) { joy[0] = 'L'; }
		else if (analog_Y >= 900) { joy[0] = 'U'; }
		else if (analog_Y <= 200) {joy[0] = 'D'; }
		else { joy[0] = 'w'; }
		
		LCD_Cursor(4,300);
		itoa(analog_Y, snum, 10);

		//if(analog_Y<200)break;
		_delay_ms(5000);
		



	}
	

}