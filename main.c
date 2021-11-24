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



enum user_input_SM {Init_user,user_on,user_off,release};

int user_input_tick(int state) {
	unsigned char Start_Button,Reset_Button,OK_button;
	Start_Button = (~PIND & 0x01);
	Reset_Button = (~PIND & 0x02);
	OK_button = (~PIND & 0x04);
	switch(state) {
		case Init_user:
		
		
		break;
		
		default:
		break;
	}
	switch(state) {
		case Init_user:
		
	
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
int display_tick(int state) {
	
	switch (state) {
		case display_init:
			LCD_init();
			LCD_ClearScreen();
		break;
		
			
		default:
		
		break;
	}

	switch(state) {
		case display_init:
		
		break;
		
	
		default:
		break;
	}
	return state;
}


int main()
{
	 DDRA = 0x00; PORTA = 0xFF; //joystick
	
	
	 DDRD = 0xFF; PORTD = 0x00;
	 
	static task task1, task2,task3,task4,task5;
	task *tasks[] = { &task1, &task2,&task3,&task4,&task5};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	
	
	// get user input button,joystick
	task1.state = Init_user;
	task1.period = 60;
	task1.elapsedTime = task1.period;
	task1.TickFct = &user_input_tick;
	
	// get sensor input battery
	task2.state =  Init_sensor;
	task2.period = 300;
	task2.elapsedTime = task2.period;
	task2.TickFct = &sensor_input_tick;
	
	// main game logic
	task3.state =  Init_game;
	task3.period = 300;
	task3.elapsedTime = task3.period;
	task3.TickFct = &InGame_tick;
	
	//lcd display
	task4.state =  display_init;
	task4.period = 300;
	task4.elapsedTime = task4.period;
	task4.TickFct = &display_tick;
	
	//indicator outputs
	task5.state =  Init_ind;
	task5.period = 300;
	task5.elapsedTime = task5.period;
	task5.TickFct = &indicator_tick;
	



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

