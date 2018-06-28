
 // Lab3P1.c
 //
 // Created: 1/30/2018 4:04:52 AM
 // Author : Eugene Rockey
 // Copyright 2018, All Rights Reserved

 #include <stdio.h>
 #include <stdlib.h>
 
 const char MS1[] = "\r\nECE-412 ATMega328P Tiny OS";
 const char MS2[] = "\r\nby Eugene Rockey Copyright 2018, All Rights Reserved";
 const char MS3[] = "\r\nMenu: (L)CD, (A)CD, (E)EPROM, (U)SART\r\n";
 const char MS4[] = "\r\nReady: ";
 const char MS5[] = "\r\nInvalid Command Try Again...";
 const char MS6[] = "Volts\r";
 
 

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

unsigned char ASCII;			//shared I/O variable with Assembly
unsigned char DATA;				//shared internal variable with Assembly
char HADC;						//shared ADC variable with Assembly
char LADC;						//shared ADC variable with Assembly

char volts[5];					//string buffer for ADC output
int Acc;						//Accumulator for ADC use

void UART_Puts(const char *str)	//Display a string in the PC Terminal Program
{
	while (*str)
	{
		ASCII = *str++;
		UART_Put();
	}
}

void LCD_Puts(const char *str)	//Display a string on the LCD Module
{
	while (*str)
	{
		DATA = *str++;
		LCD_Write_Data();
	}
}


void Banner(void)				//Display Tiny OS Banner on Terminal
{
	UART_Puts(MS1);
	UART_Puts(MS2);
	UART_Puts(MS4);
}

void HELP(void)						//Display available Tiny OS Commands on Terminal
{
	UART_Puts(MS3);
}

void LCD(void)						//Lite LCD demo
{
	DATA = 0x34;					//Student Comment Here
	LCD_Write_Command();
	DATA = 0x08;					//Student Comment Here
	LCD_Write_Command();
	DATA = 0x02;					//Student Comment Here
	LCD_Write_Command();
	DATA = 0x06;					//Student Comment Here
	LCD_Write_Command();
	DATA = 0x0f;					//Student Comment Here
	LCD_Write_Command();
	LCD_Puts("Hello ECE412!");
	/*
	Re-engineer this subroutine to have the LCD endlessly scroll a marquee sign of 
	your Team's name either vertically or horizontally. Any key press should stop
	the scrolling and return execution to the command line in Terminal. User must
	always be able to return to command line.
	*/
}

void ADC(void)	//Lite Demo of the Analog to Digital Converter
{
	volts[0x1]='.';
	volts[0x3]=' ';
	volts[0x4]= 0;
	ADC_Get();
	Acc = (((int)HADC) * 0x100 + (int)(LADC))*0xA;
	volts[0x0] = 48 + (Acc / 0x7FE);
	Acc = Acc % 0x7FE;
	volts[0x2] = ((Acc *0xA) / 0x7FE) + 48;
	Acc = (Acc * 0xA) % 0x7FE;
	if (Acc >= 0x3FF) volts[0x2]++;
	if (volts[0x2] == 58)
	{
		volts[0x2] = 48;
		volts[0x0]++;
	}
	UART_Puts(volts);
	UART_Puts(MS6);
	/*
		Re-engineer this subroutine to display temperature in degrees Fahrenheit on the Terminal.
		The potentiometer simulates a thermistor, its varying resistance simulates the
		varying resistance of a thermistor as it is heated and cooled. See the thermistor
		equations in the lab 3 folder. User must always be able to return to command line.
	*/
	
}

void EEPROM(void)
{
	UART_Puts("\r\nEEPROM Write and Read.");
	/*
	Re-engineer this subroutine so that a byte of data can be written to any address in EEPROM
	during run-time via the command line and the same byte of data can be read back and verified after the power to
	the Xplained Mini board has been cycled. Ask the user to enter a valid EEPROM address and an
	8-bit data value. Utilize the following two given Assembly based drivers to communicate with the EEPROM. You
	may modify the EEPROM drivers as needed. User must be able to always return to command line.
	*/
	UART_Puts("\r\n");
	EEPROM_Write();
	UART_Puts("\r\n");
	EEPROM_Read();
	UART_Put();
	UART_Puts("\r\n");
}

void USART(void) {
	int dataBits = 0;
	int parity = 0;
	int stopBits = 0;
	UART_Puts("\r\nSelect what you would like to change\r\n");
	UART_Puts("\r\n(1)Baud Rate (2)# of Data bits (3)Parity (4)# of Stop bits (ESC) Menu\r\n");
	UART_Get();
	switch(ASCII) {
		case '1': BAUD_Set();
			break;
		case '2': Data_Size_Set();
			break;
		case '3': Parity_Set();
			break;
		case 27:
			return;
		default: UART_Puts("Invalid Option");
			break;
	}
	return;
}

void BAUD_Set(void) {
	unsigned int done = 0;
	while (!done) {
		UART_Puts("\r\nSelect from the following Baud Rates\r\n(a) 9600 (b) 4800 (c) 2400 (d) 1200 (e) 600 (f) 300 (ESC) Menu\r\n");
		UART_Get();

		switch (ASCII) {
			case 'a':
				UART_Puts("\r\nBaud rate selected: 9600\r\n");
				asm("ldi r17, 0x00");
				asm("ldi r16, 0x67");
				done = 1;
				break;
			case 'b':
				UART_Puts("\r\nBaud rate selected: 4800\r\n");
				asm("ldi r17, 0x00");
				asm("ldi r16, 0xCF");
				done = 1;
				break;
			case 'c':
				UART_Puts("\r\nBaud rate selected: 2400\r\n");
				asm("ldi r17, 0x01");
				asm("ldi r16, 0x9F");
				done= 1;
				break;
			case 'd':
				UART_Puts("\r\nBaud rate selected: 1200\r\n");
				asm("ldi r17, 0x03");
				asm("ldi r16, 0x3F");
				done = 1;
				break;
			case 'e':
				UART_Puts("\r\nBaud rate selected: 600\r\n");
				asm("ldi 17, 0x06");
				asm("ldi r16, 0x7F");
				done = 1;
				break;
			case 'f':
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
}

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
}

void Parity_Set(void) {
	unsigned int *parity = (int *) 0xC2;
	while (1) {
		UART_Puts("\r\nSelect one of the following parity modes\r\n(D)isabled (E)ven Parity (O)dd Parity (ESC) Menu\r\n");
		UART_Get();
		switch(ASCII) {
			case 'd':
				*parity &=~(1<<5);
				*parity &=~(1<<4);
				return;
			case 'e':
				*parity |= (1<<5);
				*parity &=~(1<<4);
				return;
			case 'o':
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
}

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
}

void Command(void)					//command interpreter
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
		case: 'L': 
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
		case: 'U':
			USART();
			break;
		default:
			UART_Puts(MS5);
			HELP();
		break;  			//Add a 'USART' command and subroutine to allow the user to reconfigure the 						//serial port parameters during runtime. Modify baud rate, # of data bits, parity, 							//# of stop bits.
	}
}

int main(void)
{
	Mega328P_Init();
	Banner();
	while (1)
	{
		Command();				//infinite command loop
	}
}

