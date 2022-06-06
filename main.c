/*
 * GccApplication2.c
 *
 * Created: 2022-05-27 12:40:56
 * Author : Hanna B., Erica E. L., Ali K., Vincent L., Daniel I. V.
 */ 

#define F_CPU 8000000UL			
#include <avr/io.h>
#include <util/delay.h>
#define Data_Port	 PORTB		/* Define data port for GLCD */
#define Command_Port	 PORTD		/* Define command port for GLCD */
#define Data_Port_Dir	 DDRB		/* Define data port for GLCD */
#define Command_Port_Dir DDRD 		/* Define command port for GLCD */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define RS		 PD4		/* Define control pins */
#define RW		 PD3
#define EN		 PD5
#define CS1		 PD1
#define CS2		 PD0
#define RST		 PD2

#define TotalPage	 8
int aiplayerdown = 0;
int aiplayerup = 0;
int PlayerPosition = 0;
int aiPlayerPosition = 7;
int ballRight = 0;
int ballxPosition = 0;
int ballyPosition = 0;
int ballleft = 0;
int ballright = 0;
int balldown = 0;
int ballup = 0;
int ballRightScreen = 0;
int ballLeftScreen = 0;
int balltotalyposition = 0;
int collision = 0;
int testco = 0;

void GLCD_ClearAll()			/* GLCD all display clear function */
{
	int i,j;
	/* Select both left & right half of display */
	Command_Port |= (1 << CS1) | (1 << CS2) | (1 << RST);
	for(i = 0; i < TotalPage; i++){
		GLCD_Command((0xB8) + i);/* Increment page */
		for(j = 0; j < 64; j++){
			GLCD_Data(0);	/* Write zeros to all 64 column */
		}
	}
	GLCD_Command(0b01111111);		/* Set Y address (column=0) */
	GLCD_Command(0b10111111);		/* Set x address (page=0) */
}

void GLCD_Command(char Command){		/* GLCD command function */
	Data_Port = Command;		/* Copy command on data pin */
	Command_Port &= ~(1 << RS);	/* Make RS LOW for command register*/
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}

void GLCD_Data(char Data){		/* GLCD data function */
	Data_Port = Data;		/* Copy data on data pin */
	Command_Port |=  (1 << RS);	/* Make RS HIGH for data register */
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}

void GLCD_draw(){
	Command_Port |=  (1 << RS);	/* Make RS HIGH for data register */
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}


void GLCD_Init(){
	/* GLCD initialize function */
	Data_Port_Dir = 0xFF;
	Command_Port_Dir = 0xFF;
	/* Select both left & right half of display & Keep reset pin high */
	Command_Port |= (1 << CS1) | (1 << CS2) | (1 << RST);
	_delay_ms(20);
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_Command(0b01000000);		/* Set Y address (column=0) */
	GLCD_Command(0b10111000);		/* Set x address (page=0) */
	GLCD_Command(0xC0);		/* Set z address (start line=0) */
	GLCD_Command(0x3F);		/* Display ON */
	Command_Port |= (0 << CS1) | (0 << CS2) | (1 << RST);
	_delay_us(10);
	
}

void GLCD_leftScreen(){
	Data_Port_Dir = 0xFF;
	Command_Port_Dir = 0xFF;
	/* Select both left & right half of display & Keep reset pin high */
	Command_Port |= (1 << CS1) | (0 << CS2) | (1 << RST);
	_delay_ms(20);
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_Command(0b01000000);		/* Set Y address (column=0) */
	GLCD_Command(0b10111000);		/* Set x address (page=0) */
	GLCD_Command(0xC0);		/* Set z address (start line=0) */
	GLCD_Command(0x3F);		/* Display ON */
	Command_Port |= (0 << CS1) | (0 << CS2) | (1 << RST);
	_delay_us(10);
}

void GLCD_setxpos(char x){
	Command_Port |=  (1 << RS);	/* Make RS HIGH for data register */
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	GLCD_Command(0b10111000 + x);
	Command_Port |=  (1 << EN);	/* Make HIGH-LOW transition on Enable */
	_delay_us(2);
	Command_Port |=  (1 << EN);	/* Make HIGH-LOW transition on Enable */
}

void GLCD_setypos(char y){
	Command_Port |=  (1 << RS);	/* Make RS HIGH for data register */
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	GLCD_Command(0b01000000 + y);
	Command_Port |=  (1 << EN);	/* Make HIGH-LOW transition on Enable */
	_delay_us(2);
	Command_Port |=  (1 << EN);	/* Make HIGH-LOW transition on Enable */
}

void drawHalf(int xpos, int ypos){
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_setypos(ypos);		/* Set Y address (column=0) */
	GLCD_setxpos(xpos);
	GLCD_Command(0x3F);		/* Display ON */
		for(int x = 0; x < 5; x++){
			Data_Port=(0b11111111);
			GLCD_draw();
			_delay_us(10);
		}
	
}

void removeHalf(int xpos, int ypos){
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_setypos(ypos);		/* Set Y address (column=0) */
	GLCD_setxpos(xpos);
	GLCD_Command(0x3F);		/* Display ON */
	for(int x = 0; x < 5; x++){
		Data_Port=(0b00000000);
		GLCD_draw();
		_delay_us(10);
	}
}

void cs2high()
{
	PORTD |= 0b00000010;
}

void cs2low()
{
	PORTD &= 0b11111101;
}

void cs1high()
{
	PORTD |= 0b00000001;
}

void cs1low()
{
	PORTD &= 0b11111110;
}

void InitADC(void){
	ADMUX|=(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); //ENABLE ADC, PRESCALER 128
}

uint16_t readadc(uint8_t ch){
	ch&=0b00000111;         //ANDing to limit input to 7
	ADMUX = (ADMUX & 0xf8)|ch;  //Clear last 3 bits of ADMUX, OR with ch
	ADCSRA|=(1<<ADSC);        //START CONVERSION
	while((ADCSRA)&(1<<ADSC));    //WAIT UNTIL CONVERSION IS COMPLETE
	return(ADC);        //RETURN ADC VALUE
}

void checkJoystick(){
			uint16_t x,y;
			x=readadc(0);      //READ ADC VALUE FROM PA.0
			y=readadc(1);      //READ ADC VALUE FROM PA.1
			/* H�ger */
			if(x>611){
				right();
			}
			/* V�nster */
			if(x<610){
				left();
			}
			/* Ner */
			if(y>700){
				int xpos = PlayerPosition;
				int aixpos = aiPlayerPosition;
				down(xpos);
				aiPlayerMove(aixpos);
			}
			/* Upp */
			if(y<500){
				int xpos = PlayerPosition;
				int aixpos = aiPlayerPosition;
				up(xpos);
				aiPlayerMove(aixpos);
			}
//  			else{
//
//  			}
}

void up(int xpos){
	if(xpos == 0){
	
	}
	else{
		cs2low();
		cs1high();
		PlayerPosition = xpos - 1;
		removeHalf(xpos + 1,0);
		drawHalf(xpos,0);
		drawHalf(xpos - 1,0);
		aiplayerdown = 1;
		/*_delay_ms(20);*/
	}
}
void down(int xpos){
	if(xpos == 7){
	
	}
	else{
		cs2low();
		cs1high();
		removeHalf(xpos - 1,0);
		drawHalf(xpos,0);
		drawHalf(xpos + 1,0);
		aiplayerup = 1;
		PlayerPosition = xpos + 1;
		/*_delay_ms(20);*/
	}
}

void right(){
	
}

void left(){
	
}

void aiPlayerMove(int xpos){
		if(aiplayerdown == 1){
			if(xpos == 7){ //inverterade tempor�rt, s�tt sedan till 0
	
			}
			else{
				cs1low();
				cs2high();
				removeHalf(xpos - 1,58);
				drawHalf(xpos,58);
				drawHalf(xpos + 1,58);
				aiPlayerPosition = xpos + 1;
				aiplayerdown = 0;
				/*_delay_ms(20);*/
			}
		}
		else if(aiplayerup == 1){
			if(xpos == 0){ //inverterade tempor�rt, s�tt sedan till 7
				
			}
			else{
				cs1low();
				cs2high();
				removeHalf(xpos + 1,58);
				drawHalf(xpos,58);
				drawHalf(xpos - 1,58);
				aiPlayerPosition = xpos - 1;
				aiplayerup = 0;
				/*_delay_ms(20);*/
			}
		}
}

void drawBallRight(int ballxpos, int ballypos){
	cs1low();
	cs2high();
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_setypos(ballypos);		/* Set Y address (column=0) */
	GLCD_setxpos(ballxpos);
	GLCD_Command(0x3F);		/* Display ON */
	for(int x = 0; x < 8; x++){
		Data_Port=(0b11111111);
		GLCD_draw();
		_delay_us(10);
	}
}

void removeBallRight(int ballxpos, int ballypos){
	cs1low();
	cs2high();
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_setypos(ballypos);		/* Set Y address (column=0) */
	GLCD_setxpos(ballxpos);
	GLCD_Command(0x3F);		/* Display ON */
	for(int x = 0; x < 8; x++){
		Data_Port=(0x00);
		GLCD_draw();
		_delay_us(10);
	}	
}

void drawBallLeft(int ballxpos, int ballypos){
	cs1high();
	cs2low();
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_setypos(ballypos);		/* Set Y address (column=0) */
	GLCD_setxpos(ballxpos);
	GLCD_Command(0x3F);		/* Display ON */
	for(int x = 0; x < 8; x++){
		Data_Port=(0b11111111);
		GLCD_draw();
		_delay_us(10);
	}
}

void removeBallLeft(int ballxpos, int ballypos){
	cs1high();
	cs2low();
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_setypos(ballypos);		/* Set Y address (column=0) */
	GLCD_setxpos(ballxpos);
	GLCD_Command(0x3F);		/* Display ON */
	for(int x = 0; x < 8; x++){
		Data_Port=(0x00);
		GLCD_draw();
		_delay_us(10);
	}
}

void ballSwitchScreen(int switchtoright, int switchtoleft){
	if(switchtoright==1){
		removeBallRight(ballxPosition, ballyPosition);
		ballRightScreen=1;
		ballLeftScreen=0;
		switchtoright=0;
	}
	if(switchtoleft==1){
		removeBallLeft(ballxPosition, ballyPosition);
		ballRightScreen=0;
		ballLeftScreen=1;
		switchtoleft = 0;
	}
}

void ballnewposright(int ballxpos, int ballypos){
	if(ballxpos==0){
		ballxPosition=0;
		ballup=1;
		balldown=0;
	}
	if(ballxpos==7){
		ballxPosition=7;
		ballup=0;
		balldown=1;
	}
	if(ballyPosition>=56){
		checkforcollision();
		if(collision == 1){
			
		}
		else{
			point();
		}
		collision = 0;
	}
	if(ballup==1){
		ballxPosition+=1;
	}
	if(balldown==1){
		ballxPosition-=1;
	}
	if(ballright==1){
		ballyPosition+=8;
		balltotalyposition+=8;
	}
	if(ballleft==1){
		ballyPosition-=8;
		balltotalyposition-=8;
	}
	if(ballyPosition<8){
		ballSwitchScreen(0, 1);
		ballyPosition = 56;
	}
}

void ballnewposleft(int ballxpos, int ballypos){
	if(ballxpos==0){
		ballxPosition=0;
		ballup=1;
		balldown=0;
	}
	if(ballxpos==7){
		ballxPosition=7;
		ballup=0;
		balldown=1;
	}
	if(ballyPosition==0){
		checkforcollision();
		if(collision == 1){
			
		}
		else{
			point();
		}
		collision = 0;
	}
	if(ballup==1){
		ballxPosition+=1;
	}
	if(balldown==1){
		ballxPosition-=1;
	}
	if(ballright==1){
		ballyPosition+=8;
		balltotalyposition+=8;
	}
	if(ballleft==1){
		ballyPosition-=8;
		balltotalyposition-=8;
	}
	if(ballyPosition>56){
		ballSwitchScreen(1, 0);
		ballyPosition = 0;
	}

}

void ballmove(int ballxpos, int ballypos){
	bugcheck();
	_delay_us(10);
	if(ballRightScreen==1){
		removeBallRight(ballxpos, ballypos);
		ballnewposright(ballxpos, ballypos);
		if(ballRightScreen==1){
			drawBallRight(ballxPosition, ballyPosition);
		}
		if(ballLeftScreen==1){
			ballyPosition +=8;
			drawBallLeft(ballxPosition, ballyPosition);
			for(int x = 0; x < 8; x ++){
				removeBallLeft(x, 8);
			}
		}
	}
	if(ballLeftScreen==1){
		removeBallLeft(ballxpos, ballypos);
		ballnewposleft(ballxpos, ballypos);
		if(ballLeftScreen==1){
			drawBallLeft(ballxPosition, ballyPosition);
		}
		if(ballRightScreen==1){
			drawBallRight(ballxPosition, ballyPosition);
		}
	}
}

void cornercheck(){
	if (ballright==1 && ballyPosition == 0 && ballxPosition == 0 && ballLeftScreen == 1){
		ballxPosition = ballxPosition - 1;
		ballyPosition = ballyPosition + 8;
	}
	
	if (ballright==1 && ballyPosition == 0 && ballxPosition == 7 && ballLeftScreen == 1){
		ballxPosition = ballxPosition + 1;
		ballyPosition = ballyPosition + 8;
	}
	
	if (ballleft==1 && ballyPosition == 63 && ballxPosition == 7 && ballRightScreen == 1){
		ballxPosition = ballxPosition + 1;
		ballyPosition = ballyPosition + 8;
	}
	
	if (ballleft==1 && ballyPosition == 63 && ballxPosition == 0 && ballRightScreen == 1){
		ballxPosition = ballxPosition - 1;
		ballyPosition = ballyPosition + 8;
	}
}

void checkforcollision(){
	if(balltotalyposition == 0){
		if(PlayerPosition == ballxPosition){
			ballright = 1;
			ballleft = 0;
			balldown = 1;
			ballup = 0;
			collision = 1;
			cs1high();
			_delay_us(10);
			cs2low();
			_delay_us(10);
			drawHalf(PlayerPosition, 0);
		}
		int playersecondpos = PlayerPosition + 1;
		if(playersecondpos == ballxPosition){
			ballright = 1;
			ballleft = 0;
			balldown = 0;
			ballup = 1;
			collision = 1;
			cs1high();
			_delay_us(10);
			cs2low();
			_delay_us(10);
			drawHalf(PlayerPosition+1, 0);
		}
	}
	if(balltotalyposition == 120){
		if(aiPlayerPosition  == ballxPosition){
			ballright = 0;
			ballleft = 1;
			balldown = 1;
			ballup = 0;
			collision = 1;
		}
		int aiplayersecondpos = aiPlayerPosition + 1;
		if(aiplayersecondpos == ballxPosition){
			ballright = 0;
			ballleft = 1;
			balldown = 0;
			ballup = 1;
			collision = 1;
		}
	}
}

void point(){
	ballstop();
	/*ballspawn();*/
	cs1high();
	cs2high();
	for(int x = 0; x < 1; x++){
		for(int y = 0; y < 64; y++){
			GLCD_Command(0b01000000+y);		/* Set Y address (column=0) */
			for(int z = 0; z < 8; z++){
				GLCD_Command(0b10111000+z);		/* Set x address (page=0) */
				Data_Port=(0b11111111);
				GLCD_draw();
				_delay_us(5);
			}
		}
		_delay_ms(10);
		for(int y = 0; y < 64; y++){
			GLCD_Command(0b01000000+y);		/* Set Y address (column=0) */
			for(int z = 0; z < 8; z++){
				GLCD_Command(0b10111000+z);		/* Set x address (page=0) */
				Data_Port=(0b00000000);
				GLCD_draw();
				_delay_us(5);
			}
		}
		_delay_ms(10);
		testco = 1;
	}
}

void ballstop(){
	ballup = 0;
	balldown = 0;
	ballright = 0;
	ballleft = 0;
}

void ballspawn(){
	ballstop();
	int z = 0;
	z = rand() % 4;
	if(z==0){
		ballup = 1;
		ballleft = 1;
		ballxPosition = 3;
		ballyPosition = 64;
		ballSwitchScreen(0,1);
		balltotalyposition = 0;
	}
	if(z==1){
		balldown = 1;
		ballleft = 1;
		ballxPosition = 4;
		ballyPosition = 64;
		ballSwitchScreen(0,1);
		balltotalyposition = 0;
	}
	if(z==2){
		ballup = 1;
		ballright = 1;
		ballxPosition = 3;
		ballyPosition = 0;
		ballSwitchScreen(1,0);
		balltotalyposition = 64;

	}
	if(z==3){
		balldown = 1;
		ballright = 1;
		ballxPosition = 4;
		ballyPosition = 0;
		ballSwitchScreen(1,0);
		balltotalyposition = 64;
	}
}

void init(){
	GLCD_Init();
	GLCD_ClearAll();
	InitADC();
}

void bugcheck(){
	if (ballup == 0 && balldown == 0){
		ballup = 1;
	}
	if (ballup == 1 && balldown == 1){
		balldown = 0;
	}
	if (ballright == 0 && ballleft == 0){
		ballright = 1;
	}
	if (ballright == 1 && ballleft == 1){
		ballleft = 0;
	}
}

int main(void){
	init();
	ballspawn();
	ballmove(ballxPosition, ballyPosition);
	while (1){
		/*ballstop;*/
		checkJoystick();
		ballmove(ballxPosition, ballyPosition);
		cornercheck();
		if(testco == 1){
			GLCD_ClearAll();
			while(1){
				
			}
		}
		_delay_ms(40);
		}
}

