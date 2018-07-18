
// Lab3P1.c
//
// Created: 1/30/2018 4:04:52 AM
// Author : Eugene Rockey
// Copyright 2018, All Rights Reserved

#define F_CPU 16000000

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

const char q1[] = "If there are 6 fish and 3 drown, how many fish are there?";
const char a1 = '6';
const char q2[] = "If there are 13 apples and you take 4, how many do you have?";
const char a2 = '4';
const char q3[] = "If two's company and three's a crowd, what is 4 and 5?";
const char a3 = '9';
const char q4[] = "A cat is given $6 and a spider $12, how much for you?";
const char a4 = '3';

void LED_Init(void);
void Correct(void);
void Incorrect(void);

void LCD_Init(void);			//external Assembly functions
void UART_Init(void);
void UART_Clear(void);
void UART_Get(void);
void UART_Put(void);
void LCD_Write_Data(void);
void LCD_Write_Command(void);
void LCD_Read_Data(void);
void Mega328P_Init(void);
void Detect_Press(void);

unsigned char ASCII;			//shared I/O variable with Assembly
unsigned char DATA;				//shared internal variable with Assembly
unsigned char hAddress;			//high byte of address used in EEPROM
unsigned char lAddress;			//low byte of address used in EEPROM
unsigned char content;			//content to write in EEPROM
unsigned int count;				//counter variable

void LED_Init(void) {
	DDRC = 0x03;
	return;
}

void Correct(void) {
	PORTC = 0x01;
	return;
}

void Incorrect(void) {	
	PORTC = 0x02;
	return;
}

/**
*UART_Puts
*Display a string in the PC Terminal Program
*/
void UART_Puts(const char *str)
{
	while (*str)
	{
		ASCII = *str++;
		UART_Put();
	}
} // end UART_Puts

/**
*LCD_Puts
*Display a string on the LCD Module
*/
void LCD_Puts(const char *str)
{
	while (*str)
	{
		DATA = *str++;
		LCD_Write_Data();
	}
} // end LCD_Puts

/**
*LCD
*/
void LCD(void)						//Lite LCD demo
{
	DATA = 0x34;					//Sets data bits in LCD
	LCD_Write_Command();
	DATA = 0x08;					//Clearing display without clearing DDRAM content
	LCD_Write_Command();
	DATA = 0x02;					//Set cursor position to 0
	LCD_Write_Command();
	DATA = 0x06;					//Entry Mode
	LCD_Write_Command();
	DATA = 0x0f;					//Display on cursor blinking
	LCD_Write_Command();
	LCD_Puts(q1);
	ASCII = '\0';
	Incorrect();
	while (ASCII == '\0') {
		for (count = 0; count < 40; count ++) {
			Detect_Press();
			DATA = 0x18;
			LCD_Write_Command();
			_delay_ms(200);
			
			Detect_Press();

			//asm("lds r16,0xC6"); // check what ASCII value is being stored
			//asm("sts ASCII,r16");

			if (ASCII != '\0') {
				Correct();
				DATA = 0x01;
				LCD_Write_Command();
				return;
			}
		}
		DATA = 0x02;
		LCD_Write_Command();
	}
	Correct();
	return;
} // end LCD


/**
*USART
*Prompts user to change the Baud rate, character size, parity, stop bits, or exit back to menu
*/
void USART(void) {
	int dataBits = 0;
	int parity = 0;
	int stopBits = 0;
	UART_Puts("\r\nSelect what you would like to change\r\n");
	UART_Puts("\r\n(1)Baud Rate (2)Character Size (3)Parity (4)Stop bits (ESC) Menu\r\n");
	UART_Get();
	switch(ASCII) {
		case 27:
		return;
		default: UART_Puts("Invalid Option");
		break;
	}
	return;
} // end USART

/**
* command interpreter
*/
void Command(void)
{
	ASCII = '\0';
	while (ASCII == '\0')
	{
		UART_Get();
	}
	switch (ASCII)
	{
		case 'l':
		case 'L':
		LCD();
		break;
		case 'u':
		case 'U':
		USART();
		break;
		default:
		break;  			//Add a 'USART' command and subroutine to allow the user to reconfigure the
		//serial port parameters during runtime. Modify baud rate, # of data bits, parity,
		//# of stop bits.
	}
} // end Command

/**
* main
*/

int main(void)
{
	Mega328P_Init();
	LED_Init();

	int isRed = 0;
	LCD();
	while (!(PINB & (1<<7))) {}
	while (1)
	{
		if (!(PINB & (1<<7))) {
			if (isRed == 1) {
				Correct();
				isRed = 0;
			}
			else if (isRed == 0) {
				Incorrect();
				isRed = 1;
			}
			while (!(PINB & (1<<7))) {}
		}
	}
} // end Main

