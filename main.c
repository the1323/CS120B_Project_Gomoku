/*	Author: Tongyuan He & the033@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: UCR EE/CS 120B Fall 2021 Project Demo 2. Week 9
 *	Demo Video: https://youtu.be/EeI6JZlMs7A
 *	Exercise Description: [optional - include for your own benefit]
 *  
 *	I acknowledge all content contained herein, excluding template or example
 *	code , is my own original work.
 */
#include <avr/interrupt.h>
#include <avr/io.h>

#include <avr/eeprom.h>
#include "scheduler.h"
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif 


unsigned char current_player=1,Accept_button=0, Accept_joy=0,winner=0, writeblack=0,writewhite=0,over=0;  //1=black,2=white
unsigned char direction=0,Gcurser_x=5,Gcurser_y=5, indicator=0,menuok=0,gamejoy=0; //joystick shared
unsigned char GStart_Button,GReset_Button,GOK_Button;
unsigned short batterylife=100,bscore=0,wscore=0;
unsigned char board [12][12]; //init game board with 0, 1=black,2=white

void ADC_init(void)
{
    ADMUX |= (1<<REFS0);
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}
uint16_t ReadADC(uint8_t joy_pin)
{
    ADMUX = (ADMUX & 0xF0) | ( joy_pin & 0x03);
    ADCSRA |= (1<<ADSC);
    while( ADCSRA & (1<<ADSC) );
    return ADC;
}


void resetboard(void){
    for(int i =0;i< 12;i++){
        for(int j=0;j<12;j++){
            board[i][j]=0;
        }
    }
    Gcurser_y=Gcurser_x=5;
    bscore=wscore=winner=0;
    current_player=1;
    savegame();
}
unsigned char move (unsigned char x,unsigned char y,unsigned char player)
{
    if(board[x][y]==1 || board[x][y]==2) return 0;//invalid move
    if( board[x][y]==0){//valid
        board[x][y]=player;
        return 1;
    }
}
void loadgame(void){
    int x=0,y=0,previous=-1;
    for(int i=0;i<12;i++){
        for(int j =0;j<12;j++){
            board[i][j] = eeprom_read_byte ((const uint8_t*)(i*100+j));//i*100+j is address 
            if(board[i][j]!=1 && board[i][j] !=2) board[i][j]=0;
            
        }
        
    }
    bscore= eeprom_read_byte ((const uint8_t*)2001);
    wscore= eeprom_read_byte ((const uint8_t*)2002);
    current_player= eeprom_read_byte ((const uint8_t*)2003);
    if(bscore==255 || wscore==255|| current_player>2){
        bscore=wscore=0;
        current_player=1;
        return;
    }
    
}

void savegame(void){
    int x=0,y=0;
    for(int i=0;i<12;i++){
        for(int j =1;j<12;j++){
            eeprom_update_byte (( uint8_t *) (i*100+j), board[i][j] );
        }
    }
    eeprom_update_byte (( uint8_t *) 2001,bscore);
    eeprom_update_byte (( uint8_t *) 2002,wscore);
    eeprom_update_byte (( uint8_t *) 2003,current_player);
}

#define BLACK 0x0000
#define WHITE 0xFFFF
void show_stone(void){
    int x=0,y=0;
    for(int i=0;i<12;i++){
        for(int j =0;j<12;j++){
            LCD_Cursor(5+20*i,7+20*j);
            if(board[i][j]==1)LCD_DrawCircle(5+20*i,7+20*j,BLACK);//LCD_DrawCircle(1+20*i,5+20*j,19,19,BLACK);//LCD_WriteChar('B');
            else if(board[i][j]==2)LCD_DrawCircle(5+20*i,7+20*j,WHITE);;
        }
    }
}
int checkwin(int cx, int cy ,int xoffset,int yoffset){
    int turn = board[cx][cy];
    if (turn ==0) return 0;
    int counta=0,countb=0;
    
    for (int i = 1; i < 6; i++){// check right,down
        if(board[cx+(xoffset*i)][cy+(yoffset*i)]==turn) counta++;
        else break;
    }
    for (int i = 1; i < 6; i++){// check left,up
        if(board[cx+(xoffset*i*(-1))][cy+(yoffset*i*(-1))]==turn) countb++;
        else break;
    }
    return 1+counta+countb;
    
}
int max(int cx, int cy ){
    if(cx>cy) return cx;
    else return cy;
}
int check_alldir(int cx, int cy ){
    int xwin = checkwin(cx,cy,1,0);//x-axis
    int ywin = checkwin(cx,cy,0,1);//y-axis
    int lrwin= checkwin(cx,cy,-1,1);// '\'-axis
    int rlwin= checkwin(cx,cy,1,1);// '/'-axis
    int final = max(max(xwin,ywin),max(lrwin,rlwin)); //max of the 4
    return final;
}

enum user_input_SM {Init_user,user_press,start_hold,reset_hold,ok_hold,user_release};

int user_input_tick(int state) {
    unsigned char Start_Button,Reset_Button,OK_Button;
    Start_Button = (~PINA & 0x04);
    Reset_Button = (~PINA & 0x08);
    OK_Button = (~PINA & 0x10);
    
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
    unsigned short analog_X = ReadADC(1);//temp changed to 1
    unsigned short analog_Y = ReadADC(0);// temp changed to 0
    
    
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
            //else if (analog_Y >= 900) { direction=3;}//up
            //else if (analog_Y <= 400) {direction=4;}//down //real pos
            else if (analog_Y <= 400) { direction=3;}//up	//temp changed for this bread board
            else if (analog_Y >= 900) {direction=4;}//down
            else direction=0;
        }
        break;
    default:
        break;
    }
    return state;
}



enum InGame_SM {Init_game,Game_on};

int InGame_tick(int state) {
    
    switch(state) {
    case Init_game:
        if(menuok)state= Game_on;
        else state=Init_game;
        break;
    case Game_on:
        if(!Accept_joy || !Accept_button) state=Init_game;
        else state=Game_on;
        
        break;
        
    default:
        break;
    }
    switch(state) {
    case Init_game:
        
        
        break;
    case Game_on:
        if (Accept_joy)
        {
            menuok=0;
            if(direction==1 && Gcurser_x>0)Gcurser_x--;
            else if(direction==2 && Gcurser_x<11)Gcurser_x++;
            else if(direction==3 && Gcurser_y>1)Gcurser_y--;
            else if(direction==4 && Gcurser_y<11)Gcurser_y++;
        }
        if(GOK_Button){//put stone at current cursor
            
            if(current_player==1){
                
                if(move(Gcurser_x,Gcurser_y,current_player)){
                    int tempscore = check_alldir(Gcurser_x,Gcurser_y);
                    if(bscore<tempscore) bscore=tempscore;
                    if(tempscore>4){//game black won
                        winner=1;
                        writeblack=1;
                        over=1;
                        break;
                    }
                    
                    
                    current_player=2; //black put stone
                    savegame();
                }
                
            }
            else if(move(Gcurser_x,Gcurser_y,current_player) ){
                int tempscore = check_alldir(Gcurser_x,Gcurser_y);
                if(wscore<tempscore) wscore=tempscore;
                if(tempscore>4){//game white won
                    winner=2;
                    writewhite=1;
                    over=1;
                    break;
                }
                
                current_player=1;//white put stone
                savegame();
            }
            
            
        }
        
        
        break;
        
    default:
        break;
    }
    return state;
}



enum indicator_SM {Init_ind,ind_on};

int indicator_tick(int state) {
    
    switch(state) {
    case Init_ind:
        state=ind_on;
        break;
    case ind_on:
        state=ind_on;
    default:
        break;
    }
    switch(state) {
    case Init_ind:
        
        
        break;
    case ind_on:
        if(batterylife>30){
            indicator=1;
        }
        else indicator= !indicator;
        
        break;
    default:
        break;
    }
    return state;
}



enum display_SM { display_init,display,Game_over };
unsigned char flashstart=1,stone=1;
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
        menuok=1;
        state=display;
        break;
    case display:
        
        
        state=display;
        break;
    case Game_over:
        
        if(GStart_Button){//restart game and everything else
            if (winner){
                resetboard();
            }
            
            state=display_init;
            break;
        }
        else state=Game_over;
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
            loadgame();
            LCD_ShowPlayer(current_player);//show black/white turn
            LCD_ShowBattery(batterylife);//show battery percent
            LCD_ShowScore(0,0);
            
            if(stone){
                show_stone();
                stone=0;
            }
            
            Accept_joy=1;
        }
        
        if (Accept_joy)
        {
            if(writeblack){
                LCD_Cursor(40,160);
                LCD_WriteString("Black");
                writeblack=0;
            }
            else if (writewhite){
                LCD_Cursor(40,160);
                LCD_WriteString("White");
                writewhite=0;
            }
            LCD_ShowScore(bscore,wscore);
            show_stone();
            LCD_ShowPlayer(current_player);
        }//end if accept joy 
        
        
        if (over)
        {
            state=Game_over;
        }
        
        break;
        
    case Game_over:
        
        
        Accept_joy=0;
        LCD_Cursor(140,160);
        LCD_WriteString("Win !");
        LCD_Cursor(40,200);
        LCD_WriteString("Press START !");
        
        
        break;
    default:
        break;
        
        
        return state;
    }
}


int main()
{
    DDRA = 0x00; PORTA = 0xFF; //joystick ,buttons 
    DDRD = 0x00; PORTD = 0xFF;
    
    
    static task task1,task3,task4,task5,task6;
    task *tasks[] = { &task1, &task3,&task4,&task5,&task6};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    
    
    
    // get user input button
    task1.state = Init_user;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &user_input_tick;
    
    
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
    task5.period = 500;
    task5.elapsedTime = task5.period;
    task5.TickFct = &indicator_tick;
    
    //get user input button,joystick
    task6.state =  Init_joy;
    task6.period = 50;
    task6.elapsedTime = task6.period;
    task6.TickFct = &joystck_tick;
    
    
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

