#define F_CPU 16000000
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void LED_Digit_0(void);
void LED_Digit_1(void);
void LED_Digit_2(void);
void LED_Digit_3(void);
void LED_Digit_4(void);
void LED_Digit_5(void);
void LED_Digit_6(void);
void LED_Digit_7(void);
void LED_Digit_8(void);
void LED_Digit_9(void);

#define LED_D1()
#define LED_D2()

void Mega328P_Init(void);
void Seven_Seg_Init(void);
void UART_Put(void);
void KeyPad_Init (void);
void Read_Row_States (void);
void KeyPad_Scan (void);
void Countdown_Interrupt_Init(void);

int countdown = 60;
unsigned char ASCII;
unsigned char DATA;

ISR (TIMER1_OVF_vect) {
	TCNT1L = 0x00;
	TCNT1H = 0xD3;
}

void LED_Digit_0() {
	PORTD = 0xBD;
	PORTB |= (1<<2);
}
void LED_Digit_1() {
	PORTD = 0x24;
	PORTB &= ~(1<<2);
}
void LED_Digit_2() {
	PORTD = 0xDC;
	PORTB &= ~(1<<2);
}
void LED_Digit_3() {
	PORTD = 0xF4;
	PORTB &= ~(1<<2);
}
void LED_Digit_4() {
	PORTD = 0x65;
	PORTB |= (1<<2);
}
void LED_Digit_5() {
	PORTD = 0xF1;
	PORTB |= (1<<2);
}
void LED_Digit_6() {
	PORTD = 0xF9;
	PORTB |= (1<<2);
}
void LED_Digit_7() {
	PORTD = 0xA4;
	PORTB &= ~(1<<2);
}
void LED_Digit_8() {
	PORTD = 0xFD;
	PORTB |= (1<<2);
}
void LED_Digit_9() {
	PORTD = 0xF5;
	PORTB |= (1<<2);
}

void Seven_Seg_Init(void) {
	DDRD = 0xFF;
	DDRB = 0x07;
}

//void Digit_1(void) {
//PORTD = 0x09;
//}


void Countdown_Interrupt_Init(void) {
	TIMSK1 |= (1<<0);
	TCCR1B |= (1 << CS10) | (1<<CS12);
	TCNT1L = 0x00;
	TCNT1H = 0xD3;
}


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
	//KeyPad_Init();
	Seven_Seg_Init();
	PORTB = 0x00;
	while(1) {
		LED_Digit_9();
		_delay_ms(1000);
		LED_Digit_8();
		_delay_ms(1000);
		LED_Digit_7();
		_delay_ms(1000);
		LED_Digit_6();
		_delay_ms(1000);
		LED_Digit_5();
		_delay_ms(1000);
		LED_Digit_4();
		_delay_ms(1000);
		LED_Digit_3();
		_delay_ms(1000);
		LED_Digit_2();
		_delay_ms(1000);
		LED_Digit_1();
		_delay_ms(1000);
		LED_Digit_0();
		_delay_ms(1000);
	}
	//while(1) {
	//KeyPad_Scan();
	//}
}