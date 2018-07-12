
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
void ADC_Get(void);
void EEPROM_Read(void);
void EEPROM_Write(void);

void BAUD_Set(void);
void Data_Size_Set(void);
void Parity_Set(void);
void Stop_Set(void);
void getEEPROMAddress(void);

unsigned char ASCII;			//shared I/O variable with Assembly
unsigned char DATA;				//shared internal variable with Assembly
unsigned char hAddress;			//high byte of address used in EEPROM
unsigned char lAddress;			//low byte of address used in EEPROM
unsigned char content;			//content to write in EEPROM
unsigned int count;				//counter variable

char HADC;						//shared ADC variable with Assembly
char LADC;						//shared ADC variable with Assembly
char volts[5];					//string buffer for ADC output
int Acc;						//Accumulator for ADC use

double r, t;					// variables for temperature
char temperature[4];

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
*Lite Demo of the Analog to Digital Converter
*/
void ADC(void)
{
	ASCII = '\0';
	while (ASCII != 27) {
		ADC_Get();

		Acc = (((int)HADC) * 0x100 + (int)(LADC)); // get 10 bit number

		r = 10000.0 * Acc / (1024.0 - Acc); // adapt the thermistor to ADC

		t = 3625 * 298.15 / (298.15*log(r / 10000) + 3950); // get temperature in Kelvin
		t = t - 273.15; // convert to Celsius
		t = t * 9.0 / 5.0 + 32.0; // convert to Fahrenheit

		itoa((int)t, temperature, 10); // convert the temperature to a string for UART_Puts

		UART_Puts("\r\n");
		UART_Puts(temperature);
		UART_Puts(MS7);

		_delay_ms(100);
		
		UART_Puts("\033[2J");
		UART_Puts("\033[0;0H");

		asm("lds r16,0xC6"); // check what ASCII value is being stored
		asm("sts ASCII,r16");

	}
	return;
} // end ADC

/**
*getEEPROMAddress
*gets high-bit and then low-bit from the user when prompted
*/
void getEEPROMAddress()
{
	UART_Puts("\r\nEnter the high-bit of the EEPROM Address ");
	UART_Get();
	hAddress = ASCII; // set high bit of EEPROM address --- r18 --- see Assembler1.s
	UART_Puts("\r\nEnter the low-bit of the EEPROM Address ");
	UART_Get();
	lAddress = ASCII; // set low bit EEPROM address --- r17 --- see Assembler1.s
} // end getEEPROMAddress


/**
*EEPROM
*prompt user to read or write to EEPROM otherwise exit
*/
void EEPROM(void)
{
	UART_Puts("\r\nWould you like to (R)ead, (W)rite, or (Esc) Menu\r\n");
	UART_Get(); // get input from terminal
	if (ASCII == 'r' || ASCII == 'R')
	{
		UART_Puts("\r\nYou selected to read from EEPROM\r\n");
		getEEPROMAddress();
		UART_Puts("\r\n\r\nContent at ");
		ASCII = hAddress;
		UART_Put();
		UART_Puts("x");
		ASCII = lAddress;
		UART_Put();
		UART_Puts(": ");
		EEPROM_Read(); // gets value stored in EEPROM
		UART_Put();
		UART_Puts("\r\n");
	}
	else if (ASCII == 'w' || ASCII == 'W')
	{
		UART_Puts("\r\nYou selected to write to EEPROM\r\n");
		getEEPROMAddress();
		UART_Puts("\r\nEnter the content to be stored at the EEPROM Address provide\r\n");
		UART_Get(); // get input from terminal
		content = ASCII; // set content to be stored in address
		EEPROM_Write(); // write data to EEPROM
	}
	else if (ASCII == 27)
	{
		UART_Puts("\r\nThank you.\r\n");
		return;
	}
	else
	{
		UART_Puts(MS5);
	}
} // end EEPROM

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
		case '1': BAUD_Set();
			break;
		case '2': Data_Size_Set();
			break;
		case '3': Parity_Set();
			break;
		case '4': Stop_Set();
			break;
		case 27:
			return;
		default: UART_Puts("Invalid Option");
			break;
	}
	return;
} // end USART

/**
*BAUD_Set
*Changes the Baud rate to predefined values based on user choice
*/
void BAUD_Set(void) {
	unsigned int done = 0;
	while (!done) {
		UART_Puts("\r\nSelect from the following Baud Rates\r\n(a) 9600 (b) 4800 (c) 2400 (d) 1200 (e) 600 (f) 300 (ESC) Menu\r\n");
		UART_Get();

		switch (ASCII) {
			case 'a':
			case 'A':
				UART_Puts("\r\nBaud rate selected: 9600\r\n");
				asm("ldi r17, 0x00");
				asm("ldi r16, 0x67");
				done = 1;
				break;
			case 'b':
			case 'B':
				UART_Puts("\r\nBaud rate selected: 4800\r\n");
				asm("ldi r17, 0x00");
				asm("ldi r16, 0xCF");
				done = 1;
				break;
			case 'c':
			case 'C':
				UART_Puts("\r\nBaud rate selected: 2400\r\n");
				asm("ldi r17, 0x01");
				asm("ldi r16, 0x9F");
				done= 1;
				break;
			case 'd':
			case 'D':
				UART_Puts("\r\nBaud rate selected: 1200\r\n");
				asm("ldi r17, 0x03");
				asm("ldi r16, 0x3F");
				done = 1;
				break;
			case 'e':
			case 'E':
				UART_Puts("\r\nBaud rate selected: 600\r\n");
				asm("ldi 17, 0x06");
				asm("ldi r16, 0x7F");
				done = 1;
				break;
			case 'f':
			case 'F':
				UART_Puts("\r\nBaud rate selected: 300\r\n");
				asm("ldi r17, 0x0C");
				asm("ldi r16, 0xFF");
				done = 1;
				break;
			case 27:
				return;
			default:		
				UART_Puts("\r\nInvalid option\r\n");
				break;
		}
	}

	asm("sts 0xC5,r17");
	asm("sts 0xC4,r16");
	
	return;
} // end BAUD_Set

/**
*Data_Size_Set
*Sets the character size to predefined values based on user input
*/
void Data_Size_Set(void) {	
	unsigned int *b_reg = (int *) 0xC1;
	unsigned int *c_reg = (int *) 0xC2;
	while (1) {
		UART_Puts("\r\nSelect from the following bit sizes\r\n(5) bits (6) bits (7) bits (8) bits (9) bits (ESC) Menu\r\n");
		UART_Get();
		switch (ASCII) {
			case '5':
				UART_Puts("\r\n5 bits\r\n");
				*b_reg &=~(1<<2);
				*c_reg &=~(1<<2);
				*c_reg &=~(1<<2);
				return;
			case '6':
				UART_Puts("\r\n6 bits\r\n");
				*b_reg &=~(1<<2);
				*c_reg &=~(1<<2);
				*c_reg |= 1<<2;
				return;
			case '7':
				UART_Puts("\r\n7 bits\r\n");
				*b_reg &=~(1<<2);
				*c_reg |= 1<<2;
				*c_reg &=~(1<<1);
				return;
			case '8':
				UART_Puts("\r\n8 bits\r\n");
				*b_reg &=~(1<<2);
				*c_reg |= (1<<2);
				*c_reg |= (1<<1);
				return;
			case '9':
				UART_Puts("\r\n9 bits\r\n");
				*b_reg |= 1<<2;
				*c_reg |= (1<<2);
				*c_reg |= (1<<1);
				return;
			case 27:
				return;
			default:
				break;
		}
	}
} // end Data_Size_Set

/*
*Parity_Set
*Changes the parity bit to predefined values based on user choice
*/
void Parity_Set(void) {
	unsigned int *parity = (int *) 0xC2;
	while (1) {
		UART_Puts("\r\nSelect one of the following parity modes\r\n(D)isabled (E)ven Parity (O)dd Parity (ESC) Menu\r\n");
		UART_Get();
		switch(ASCII) {
			case 'd':
			case 'D':
				*parity &=~(1<<5);
				*parity &=~(1<<4);
				return;
			case 'e':
			case 'E':
				*parity |= (1<<5);
				*parity &=~(1<<4);
				return;
			case 'o':
			case 'O':
				*parity |= (1<<5);
				*parity |= (1<<4);
				return;
			case 27:
				return;
			default:
				UART_Puts("\r\nInvalid option\r\n");
				break;
		}
	}
} // end Parity_Set

/*
*Stop_Set
*Changes the stop bit to predefined values based on user choice
*/
void Stop_Set (void) {
	unsigned int data = (int *) 0xC2;
	while (1) {
		UART_Puts("\r\nSelect one of the following number of stop bits\r\n(1) bit (2) bits (ESC) Menu\r\n");
		UART_Get();
		switch (ASCII) {
			case '1':
				data &=~(1<<3);
				return;
			case '2':
				data |= 1<<3;
				return;
			case 27:
				return;
			default:
				UART_Puts("\r\nInvalid option\r\n");
				break;
		}
	}
} // end Stop_Set

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
		case 'a':
		case 'A': 
			ADC();
			break;
		case 'e':
		case 'E': 
			EEPROM();
			break;
		case 'u': 
		case 'U':
			USART();
			break;
		default:
			UART_Puts(MS5);
			HELP();
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

