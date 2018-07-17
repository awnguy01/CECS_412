
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

const char MS1[] = "\r\nECE-412 ATMega328P Tiny OS";
const char MS2[] = "\r\nby Eugene Rockey Copyright 2018, All Rights Reserved";
const char MS3[] = "\r\nMenu: (L)CD, (A)DC, (E)EPROM, (U)SART\r\n";
const char MS4[] = "\r\nReady: ";
const char MS5[] = "\r\nInvalid Command Try Again...";
const char MS6[] = "Volts\r";
const char MS7[] = " F";
const char teamName[] = "               Four is Better Than Five";

void LCD_Init(void);			//external Assembly functions
void UART_Init(void);
void UART_Clear(void);
void UART_Get(void);
void UART_Put(void);
void LCD_Write_Data(void);
void LCD_Write_Command(void);
void LCD_Read_Data(void);
void Mega328P_Init(void);

unsigned char ASCII;			//shared I/O variable with Assembly
unsigned char DATA;				//shared internal variable with Assembly
unsigned char hAddress;			//high byte of address used in EEPROM
unsigned char lAddress;			//low byte of address used in EEPROM
unsigned char content;			//content to write in EEPROM
unsigned int count;				//counter variable

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
*Banner
*Display Tiny OS Banner on Terminal
*/
void Banner(void)
{
	UART_Puts(MS1);
	UART_Puts(MS2);
	UART_Puts(MS4);
} // end Banner

/**
*HELP
*Display available Tiny OS Commands on Terminal
*/
void HELP(void)
{
	UART_Puts(MS3);
} // end HELP

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
	LCD_Puts(teamName);
	ASCII = '\0';
	while (ASCII == '\0') {
		for (count = 0; count < 40; count ++) {
			DATA = 0x18;
			LCD_Write_Command();
			_delay_ms(200);
			
			asm("lds r16,0xC6"); // check what ASCII value is being stored
			asm("sts ASCII,r16");

			if (ASCII != '\0') {
				DATA = 0x01;
				LCD_Write_Command();
				return;
			}
		}
		DATA = 0x02;
		LCD_Write_Command();
	}
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
	UART_Puts(MS3);
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
		UART_Puts(MS5);
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
	Banner();
	while (1)
	{
		Command();				//infinite command loop
	}
} // end Main

