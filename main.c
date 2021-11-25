/*	Author: Tongyuan He & the033@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #11  Exercise #5
 *	Demo Video: https://youtu.be/PkV9gSkrTFg
 *             Full Playlist: https://www.youtube.com/playlist?list=PLGUGavrXPSvGQtA9dd-Y3l1FxD06JdU04
 *	Exercise Description: [optional - include for your own benefit]
 *  
 *	I acknowledge all content contained herein, excluding template or example
 *	code , is my own original work.
 */
#include <avr/interrupt.h>
#include <avr/io.h>
//#include "io.h"
//#include "keypad.h"
#include "scheduler.h"
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif 

unsigned char current_player=1,Accept_button=0, Accept_joy=0;  //1=black,2=white
unsigned char direction=0,Gcurser_x=5,Gcurser_y=5; //joystick shared
unsigned char GStart_Button,GReset_Button,GOK_Button;
unsigned short batterylife=100,bscore=0,wscore=10;
unsigned char board [12][12] ={0}; //init game board with 0, ps: 1=black,2=white
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
unsigned char move (unsigned char x,unsigned char y,unsigned char player)
{
	if(board[x][y]==1 || board[x][y]==2) return 0;//invalid move
	if( board[x][y]==0){//valid
		board[x][y]=player;
		return 1;
	}
}

void show_stone(void){
	int x=0,y=0;
	for(int i=0;i<12;i++){
		for(int j =0;j<12;j++){
			LCD_Cursor(5+20*i,7+20*j);
			if(board[i][j]==1)LCD_WriteChar('B');
			else if(board[i][j]==2)LCD_WriteChar('W');
		}
	}
}

enum user_input_SM {Init_user,user_press,start_hold,reset_hold,ok_hold,user_release};

int user_input_tick(int state) {
	unsigned char Start_Button,Reset_Button,OK_Button;
	Start_Button = (~PIND & 0x01);
	Reset_Button = (~PIND & 0x02);
	OK_Button = (~PIND & 0x04);
	
	switch(state) {
		case Init_user:
			if (Accept_button){	
				state=user_release;
			}
			else state=Init_user;
		break;
		case user_release:
			GStart_Button=GReset_Button=GOK_Button=0;
			if (Accept_button){
				if(Start_Button && !Reset_Button && ! OK_Button){
					state= start_hold;
				}
				else if(!Start_Button && Reset_Button && ! OK_Button){
					state= reset_hold;
				}
				else if(!Start_Button && !Reset_Button &&  OK_Button){
					state= ok_hold;
				}
			}
			else state=user_release;
		break;
		case start_hold:
			if (!Start_Button){	
				GStart_Button=1;
				state=user_release;
			}
			else state = start_hold;
		break;
		
		case reset_hold:
		if (!Reset_Button){
			GReset_Button=1;
			state=user_release;
		}
		else state = reset_hold;
		break;
		
		case ok_hold:
		if (!OK_Button){
			GOK_Button=1;
			state=user_release;
		}
		else state = ok_hold;
		break;
		
		default:
		break;
	}
	switch(state) {
		case Init_user:
		
		break;
		case user_release:
			
		break;
		
		default:
		break;
	}
	return state;
}

	


enum joystck_SM {Init_joy,joy_action};
	
int joystck_tick(int state) {
	unsigned short analog_X = ReadADC(0);
	unsigned short analog_Y = ReadADC(1);
	
	
	switch(state) {
		case Init_joy:
			if(Accept_joy){
				
				ADC_init();
				state=joy_action;
				
			}
			
		break;
		case joy_action:
			if(!Accept_joy)state=Init_joy;
			else state=joy_action;
		
		default:
		break;
	}
	switch(state) {
		case Init_joy:
			
		
		break;
		case joy_action:
			if(Accept_joy){
				
				if (analog_X <= 400) { direction=1;} //left
				else if (analog_X >= 900) { direction=2;}//right
				else if (analog_Y >= 900) { direction=3;}//up
				else if (analog_Y <= 400) {direction=4;}//down
				else direction=0;
			}
		break;
		default:
		break;
	}
	return state;
}



enum sensor_input_SM {Init_sensor,sensor_on,sensor_off};

int sensor_input_tick(int state) {
	
	
	switch(state) {
		case Init_sensor: 
		 
		
		
		break;
		
		default:
		break;
	}
	switch(state) {
		case Init_sensor:
		
		
		break;
		
		default:
		break;
	}
	return state;
}



enum InGame_SM {Init_game,Game_on,Game_over};

int InGame_tick(int state) {

	switch(state) {
		case Init_game:
		
		break;
		
		default:
		break;
	}
	switch(state) {
		case Init_game:
		
		
		break;
		
		default:
		break;
	}
	return state;
}

	

enum indicator_SM {Init_ind,ind_on,ind_over};

int indicator_tick(int state) {

	switch(state) {
		case Init_ind:
		
		break;
		
		default:
		break;
	}
	switch(state) {
		case Init_ind:
		
		
		break;
		
		default:
		break;
	}
	return state;
}




enum display_SM { display_init,display };
unsigned char flashstart=1;
int display_tick(int state) {
	
	switch (state) {
		case display_init:
		
				LCD_init();
				LCD_ClearScreen();

				
				LCD_Cursor(40,40);
				LCD_WriteString("Let's go");
				LCD_Cursor(120,80);
				LCD_WriteString("Gomoku!!");
				LCD_Cursor(40,200);
				LCD_WriteString("Press START !");
				
				LCD_Cursor(40,280);
				LCD_WriteString("CS120B Project");
				LCD_Cursor(40,300);
				LCD_WriteString("    -- The033");
				Accept_button=1;
			state=display;
		break;
		case display:
			
		
		state=display;
		break;
		
			
		default:
		
		break;
	}

	switch(state) {
		case display_init:
			
		break;
		case display:
			if(!GStart_Button && !Accept_joy){ //flash "Press START !" while no input
				
				LCD_Cursor(40,200);
				if(flashstart){LCD_WriteString("Press START !"); flashstart=0;}
				else{LCD_WriteString("              "); flashstart=1;}
				break;	
			}
			if(GStart_Button && !Accept_joy){//game start
				
				
				LCD_ClearScreen();
				
				LCD_DrawBoard();
				LCD_ShowPlayer(1);//show black/white turn
				LCD_ShowBattery(100);//show battery percent
				LCD_ShowScore(0,0);
				Accept_joy=1;
				
			}
			if(Accept_joy){//ingame
				
				if(direction==0){ //move gcurser by joystick
					LCD_GameCursor(Gcurser_x,Gcurser_y);
				}
				else{
					LCD_Cursor(5+20*Gcurser_x,5+20*Gcurser_y);
					LCD_WriteString(" " );
					if(direction==1 && Gcurser_x>0)Gcurser_x--;
					else if(direction==2 && Gcurser_x<11)Gcurser_x++;
					else if(direction==3 && Gcurser_y>1)Gcurser_y--;
					else if(direction==4 && Gcurser_y<11)Gcurser_y++;
					
					LCD_GameCursor(Gcurser_x,Gcurser_y);
				}
				if(GReset_Button){//reset game and everything else
					Accept_button=0;
					Accept_joy=0;
					state=display_init;
					break;
				}
				if(GOK_Button){//put stone at current cursor
					//LCD_Cursor(2,260);
					//LCD_WriteString("okok" );
					if(current_player==1){
						if(move(Gcurser_x,Gcurser_y,current_player))
						 current_player=2; //black put stone
					}
					else if(move(Gcurser_x,Gcurser_y,current_player) ) current_player=1;//white put stone
					
				}
				show_stone();
				LCD_ShowPlayer(current_player);
			}//end if accept joy 
			
			
			
			
		break;
	
		default:
		break;
	}
	return state;
}


int main()
{
	 DDRA = 0x00; PORTA = 0xFF; //joystick
	
	
	 DDRD = 0x00; PORTD = 0xFF;//buttons
	 
	static task task1, task2,task3,task4,task5,task6;
	task *tasks[] = { &task1, &task2,&task3,&task4,&task5,&task6};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	
	
	// get user input button
	task1.state = Init_user;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &user_input_tick;
	
	// get sensor input battery
	task2.state =  Init_sensor;
	task2.period = 100;
	task2.elapsedTime = task2.period;
	task2.TickFct = &sensor_input_tick;
	
	// main game logic
	task3.state =  Init_game;
	task3.period = 100;
	task3.elapsedTime = task3.period;
	task3.TickFct = &InGame_tick;
	
	//lcd display
	task4.state =  display_init;
	task4.period = 100;
	task4.elapsedTime = task4.period;
	task4.TickFct = &display_tick;
	
	//indicator outputs
	task5.state =  Init_ind;
	task5.period = 100;
	task5.elapsedTime = task5.period;
	task5.TickFct = &indicator_tick;
	
	//get user input button,joystick
	task6.state =  Init_joy;
	task6.period = 50;
	task6.elapsedTime = task6.period;
	task6.TickFct = &joystck_tick;


	//LCDGAMEINTI();
	TimerSet(10);
	TimerOn();
	
	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 10;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return 0;
}	

