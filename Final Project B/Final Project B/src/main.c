#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h> 

void Mega328P_Init(void);
void UART_Put(void);
void KeyPad_Init (void);
void Read_Row_States (void);
void KeyPad_Scan (void);

unsigned char ASCII;
unsigned char DATA;

//row and column mapped keypad grid with placeholder 0s
const int KeyPadGrid[4][4] = 
{
	{0, 0, 0, 0},
	{0, 1, 2, 3},
	{0, 4, 5, 6},
	{0, 7, 8, 9}
};

const int row2Vals[] = {0, 4, 5, 6};
const int row3Vals[] = {0, 7, 8, 9};

int row;
int col;
int connected = 0;

void KeyPad_Init (void) {

	DDRD &= ~((1<<0) | (1<<1) | (1<<2)); //Keypad rows set to input
	DDRD |= (1<<3) | (1<<4) | (1<<5);	//Keypad columns set to output
	PORTD = 0x00;	//Keypad all set to low
}

void Read_Row_States(void) {
	if ((PIND & (1<<0)) != 0) {
		row = 0;
		connected = 1;
	} else if ((PIND & (1<<1)) != 0) {
		row = 1;
		connected = 1;
	} else if ((PIND & (1<<2)) != 0) {
		row = 2;
		connected = 1;
	} else {
		connected = 0;
	}
}

void KeyPad_Scan(void) {
	int key = 0;
	while (connected == 0) {
		if (connected == 0) {
			//Sending high from col 3 and scanning for matching row	
			PORTD = 8;
			col = 3;
			Read_Row_States();
		}
		if (connected == 0) {
			//Sending high from col 2 and scanning for matching row
			PORTD = 16;
			col = 2;
			Read_Row_States();
		}
		if (connected == 0) {
			//Sending high from col 1 and scanning for matching row
			PORTD = 32;
			col = 1;
			Read_Row_States();
		}
	}
	key = KeyPadGrid[row][col];
	ASCII = key + '0';
	UART_Put();
	row = NULL;
	col = NULL;
}

int main(void) {
	Mega328P_Init();
	KeyPad_Init();
	while(1) {
		KeyPad_Scan();
	}
}