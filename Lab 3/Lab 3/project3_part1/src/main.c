// Lab3P1.c
//
// Created: 1/30/2018 4:04:52 AM
// Author : Eugene Rockey
// Copyright 2018, All Rights Reserved
 
//no includes, no ASF, no libraries

#include <math.h>
#include <stdlib.h>
 
const char MS1[] = "\r\nECE-412 ATMega328P Tiny OS";
const char MS2[] = "\r\nby Eugene Rockey Copyright 2018, All Rights Reserved";
const char MS3[] = "\r\nMenu: (L)CD, (A)CD, (E)EPROM\r\n";
const char MS4[] = "\r\nReady: ";
const char MS5[] = "\r\nInvalid Command Try Again...";
const char MS6[] = "Volts\r";
const char MS7[] = " F";
const char teamName[] = "Four is Better Than Five";

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

unsigned char ASCII;		//shared I/O variable with Assembly
unsigned char DATA;			//shared internal variable with Assembly
unsigned char hAddress; //high byte of address used in EEPROM
unsigned char lAddress; //low byte of address used in EEPROM
unsigned char content;  //content to write in EEPROM

char HADC;					   	//shared ADC variable with Assembly
char LADC;					    //shared ADC variable with Assembly
char volts[5];					//string buffer for ADC output

int Acc;						    //Accumulator for ADC use

double r, t; // variables for temperature
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
} // end help

/**
*LCD
*/
void LCD(void) // Lite LCD demo
{
  DATA = 0x34; //Student Comment Here
  LCD_Write_Command();
  DATA = 0x08; //Student Comment Here
  LCD_Write_Command();
  DATA = 0x02; //Student Comment Here
  LCD_Write_Command();
  DATA = 0x06; //Student Comment Here
  LCD_Write_Command();
  DATA = 0x0f; //Student Comment Here
  LCD_Write_Command();
  LCD_Puts(teamName);
	/*
	Re-engineer this subroutine to have the LCD endlessly scroll a marquee sign of 
	your Team's name either vertically or horizontally. Any key press should stop
	the scrolling and return execution to the command line in Terminal. User must
	always be able to return to command line.
	*/
} // end LCD

/**
*LCDScroll
*/
void LCDScroll(void)
{
	DATA = 0x18;						                // command to scroll the text on LCD
	LCD_Write_Command();				            // send the command to the LCD
  UART_Puts("Team name is scrolling.");	  // place space into terminal
  LCD_Write_Command();		                // send the command to the LCD
  LCD_Write_Data();
} // end LCDScroll

/**
*Lite Demo of the Analog to Digital Converter
*/
void ADC(void)
{
  ASCII = '\0';
  while (ASCII != 'e' || ASCII != 'E') {
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
    UART_Puts("\033[2J");
    UART_Puts("\033[0;0H");

    asm("lds r16,0xC6"); // check what ASCII value is being stored 
    asm("sts ASCII,r16");

    if (ASCII == 'e') {
      return;
    }
  }
  
} // end ADC

/**
*getEEPROMAddress
*gets high-bit and then low-bit from the user when prompted
*/
void getEEPROMAddress()
{
  UART_Puts("\nEnter the high-bit of the EEPROM Address: ");
  UART_Get();
  hAddress = ASCII; // set high bit of EEPROM address --- r18 --- see Assembler1.s
  UART_Puts("Enter the low-bit of the EEPROM Address: ");
  UART_Get();
  lAddress = ASCII; // set low bit EEPROM address --- r17 --- see Assembler1.s
} // end getEEPROMAddress

/**
*EEPROM
*prompt user to read or write to EEPROM otherwise exit
*/
void EEPROM(void)
{
  UART_Puts("Would you like to (R)ead, (W)rite, or (E)xit: ");
  UART_Get(); // get input from terminal
  if (ASCII == 'r' || ASCII == 'R') 
  {
    UART_Puts("You selected to read from EEPROM.\n");
    getEEPROMAddress();
    EEPROM_Read(); // gets value stored in EEPROM
  } 
  else if (ASCII == 'w' || ASCII == 'W') 
  {
    UART_Puts("You selected to write to EEPROM.\n");
    getEEPROMAddress();
    UART_Puts("Enter the content to be stored at the EEPROM Address provide: ");
    UART_Get(); // get input from terminal
    content = ASCII; // set content to be stored in address
    EEPROM_Write(); // write data to EEPROM
  }
  else if (ASCII == 'e' || ASCII == 'E') 
  {
    UART_Puts("Thank you.");
    return;
  } 
  else 
  {
    UART_Puts(MS5);
  }
} // end EEPROM

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
    case 'L' | 'l': LCD();
    break;
    case 'A' | 'a': ADC();
    break;
    case 'E' | 'e': EEPROM();
    break;
    default:
    UART_Puts(MS5);
    HELP();
    break; //Add a 'USART' command and subroutine to allow the user to reconfigure the 						
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
	  Command(); //infinite command loop
  }
} // end main

