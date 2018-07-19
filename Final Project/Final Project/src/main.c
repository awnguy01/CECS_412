
// Lab3P1.c
//
// Created: 1/30/2018 4:04:52 AM
// Author : Eugene Rockey
// Copyright 2018, All Rights Reserved

#define F_CPU 16000000

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

const char q1[] = "If there are 6 fish and 3 drown, how many fish are there?";
const int a1 = 6;
const char q2[] = "If there are 13 apples and you take 4, how many do you have?";
const int a2 = 4;
const char q3[] = "If two's company and three's a crowd, what is 4 and 5?";
const int a3 = 9;
const char q4[] = "A cat is given $6 and a spider $12, how much for you?";
const int a4 = 3;
const char q5[] = "Among the numbers 1-10, what number is most likely the fattest?";
const int a5 = 7;

int questions[] = {1, 2, 3, 4, 5}; // array of questions represented by numbers. i.e. 1 = q1 and hence 1 => a1
int answer; /* answer that the user provides to be checked against respective GeneratedCode digit
             * TODO: check if users input is going to be a char instead of int
             */

void LED_Init(void);
void onCorrect(void);
void onIncorrect(void);

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

void UART_Puts(const char*);
void LCD_Puts(const char*);
void LCD(const char*);
void USART(void);
void Command(void);

void GenerateQuestionOrder(int[], int);
void AskQuestions(int[]);
void Swap(int *, int *);
void PrintQuestionOrder(int[], int);
void PromptForAnswer(const char[], int);

unsigned char ASCII;			//shared I/O variable with Assembly
unsigned char DATA;				//shared internal variable with Assembly
unsigned char hAddress;			//high byte of address used in EEPROM
unsigned char lAddress;			//low byte of address used in EEPROM
unsigned char content;			//content to write in EEPROM
unsigned int count;				//counter variable

void LED_Init(void) {
	DDRC = 0x03;				//Sets data direction of PC0 and PC1 to output to send "high" to LEDs
	return;
}

void onCorrect(void) {
	PORTC = 0x01;				//Sends a "high" signal to PC0 and "low" for every other bit in PORTC
	return;
}

void onIncorrect(void) {	
	PORTC = 0x02;				//Sends a "high" signal to PC1 and "low" for every other bit in PORTC
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
	_delay_ms(200);
	DATA = 0x01;
	LCD_Write_Command();
	_delay_ms(5);
	DATA = 0x06;
	LCD_Write_Command();
	_delay_ms(5);
	while (*str)
	{
		DATA = *str++;
		LCD_Write_Data();
	}
	_delay_ms(5);
} // end LCD_Puts

void LCD_Init() {
	DATA = 0x38;					//Sets data bits in LCD
	LCD_Write_Command();
	DATA = 0x08;					//Clearing display without clearing DDRAM content
	LCD_Write_Command();
	DATA = 0x02;					//Set cursor position to 0
	LCD_Write_Command();
	DATA = 0x06;					//Entry Mode
	LCD_Write_Command();
	DATA = 0x0c;					//Display on cursor blinking
	LCD_Write_Command();
};

/**
*LCD
*/
void LCD(const char q[])						//Lite LCD demo
{
	LCD_Puts(q);
	ASCII = '\0';
	while (ASCII == '\0') {
		for (count = 0; count < 40; count ++) {
			//Detect_Press();
			DATA = 0x18;
			LCD_Write_Command();
			_delay_ms(200);
			
			//Detect_Press();			//Loads ASCII with 1 if PB7 is "high" (i.e. Pushbutton pressed)

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
 * GenerateQuestionOrder
 * a function to generate a random permutation of an array of question. the first four values in the array will be used
 * these questions correspond answers and therefore the code. i.e. questions: 2 1 5 3 =>  q2 q1 q5 q3 => a2 a1 a5 a3 => code: 4 6 7 9
 */
void GenerateQuestionOrder(int questions[], int n) 
{
  // start from the last element and swap one by one. don't need to do it for the first element
  for (int i = (n - 1); i > 0; i--) {
    int j = (rand() % (i + 1)); // pick a random index from 0 to i      
    Swap(&questions[i], &questions[j]); // Swap question[i] with the element at random index
  }
} // end GenerateQuestionOrder

/**
 * Swap
 * a utility function to swap two integers
 */
void Swap(int *a, int *b) 
{
  int temp = *a;
  *a = *b;
  *b = temp;
} // end Swap

/**
 * PrintQuestionOrder
 * function that 'prints' the questions
 * the correct code is the corresponding questions and answers
 * i.e. questions: 2 1 5 3 =>  q2 q1 q5 q3 => a2 a1 a5 a3 => code: 4 6 7 9
 * TODO: replace with UART_Puts or LCD? most likely remove this?
 */
void PrintQuestionOrder(int questions[], int n) 
{
  for (int i = 0; i < n; i++) {
    printf("%d ", questions[i]);
  }

  printf("\n");
} // end PrintQuestionOrder

/**
 * PromptForAnswer
 * TODO: replace printf and scanf with LCD_Put and LCD_Get
 * TODO: inside if (answer != a) decrement timer 5 seconds
 * TODO: add check to while condition for timer > 0
 */
void PromptForAnswer(const char q[], int a)
{
	ASCII = '\0';
	onIncorrect();
	while (ASCII != a + '0') {
		LCD_Puts(q);
		while (ASCII == '\0') {
			UART_Get();
		}
		if (ASCII == a + '0') {
			_delay_ms(200);
			onCorrect();
			_delay_ms(1000);
		} else {
			LCD_Puts("Wrong try again");
			_delay_ms(5000);
			ASCII = '\0';
			UCSR0A &= ~(1<<7);
		}
	}
	return;
} // end PromptForAnswer

/**
 * AskQuestion
 * display question to the user and get answer until time runs out or user enters correct code
 * only asks the user the first four questions in the shuffled array of questions => four digit code
 * TODO: subtract 5 seconds from timer when answer is incorrect
 * TODO: add flavor text?
 */
void AskQuestions(int questions[]) 
{
  for (int i = 0; i < 4; i++) {
    if (questions[i] == 1) {
      PromptForAnswer(q1, a1);
    } else if (questions[i] == 2) {
      PromptForAnswer(q2, a2);
    } else if (questions[i] == 3) {
      PromptForAnswer(q3, a3);
    } else if (questions[i] == 4) {
      PromptForAnswer(q4, a4);
    } else if (questions[i] == 5) {
      PromptForAnswer(q5, a5);
    }
  }
  LCD_Puts("Next time...");
  onCorrect();
  return;
} // end AskQuestion

/**
* main
*/

int main(void)
{
	srand(time(NULL));
	GenerateQuestionOrder(questions, 4);

	Mega328P_Init();
	LED_Init();
	LCD_Init();
	onIncorrect();	//Initially LED red
	//while (!(PINB & (1<<7))) {}			//Used for debouncing of pushbutton
	
	AskQuestions(questions);
		//if (!(PINB & (1<<7))) {
			//if (isRed == 1) {
				//onCorrect();
				//isRed = 0;
			//}
			//else if (isRed == 0) {
				//onIncorrect();
				//isRed = 1;
			//}
			//while (!(PINB & (1<<7))) {}	//Used for debouncing of pushbutton
		//}
} // end Main

