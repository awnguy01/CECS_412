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
void LED_Disp_1(void);
void LED_Disp_2(void);
void Tens_Display(void);
void Ones_Display(void);

void Mega328P_Init(void);
void FX_Init(void);
void UART_Put(void);
void KeyPad_Init (void);
void Read_Row_States (void);
void KeyPad_Scan (void);
void Countdown_Interrupt_Init(void);

int countdown = 60;
int go = 1;
unsigned char ASCII;
unsigned char DATA;

ISR (TIMER2_OVF_vect) {
	//Interrupt that flickers the 7-segment LED
	Ones_Display();
	Tens_Display();
	TCNT2 = 0xFF;
}

ISR (TIMER1_OVF_vect) {
	//Interrupt that executes each segment
	PORTB |= (1<<3);
	//Counts down only if the start signal pin is high
	if (PINC & (1<<0)) {
		if (countdown > 0) {
			countdown--;
		}
		else {
			PORTB |= (1<<4);
			TIMSK1 = 0;
		}
		PORTB |= (1<<3);
		_delay_ms(200);
	}
	PORTB &= ~(1<<3);
	TCNT1L = 0x00;
	TCNT1H = 0xD3;
}

//Digit segment mapping for 4-digit 7-segment LED
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

void LED_Disp_1() {
	PORTB &= ~(1<<0);
	PORTB |= (1<<1);
}

void LED_Disp_2() {
	PORTB &= ~(1<<1);
	PORTB |= (1<<0);
}

void FX_Init(void) {
	//Initialize the timer ports to output
	DDRD = 0xFF;
	DDRB = 0xFF;
	//Initialize the the start signal port to input
	DDRC &= ~(1<<5);
	PORTC &= ~(1<<5);
}


void Countdown_Interrupt_Init(void) {
	TIMSK1 |= (1<<0);
	TIMSK2 |= (1<<0);
	TCCR1B |= (1 << CS10) | (1<<CS12);
	TCCR2B |= (1<<0) | (1<<1) | (1<<2);
	TCNT1L = 0x00;
	TCNT1H = 0xD3;
	TCNT2 = 0xFF;
	PORTB = 0x00;
}

void Tens_Display(void) {
	LED_Disp_1();
	switch((countdown / 10) % 10) {
		case 0:
		LED_Digit_0();
		break;
		case 1:
		LED_Digit_1();
		break;
		case 2:
		LED_Digit_2();
		break;
		case 3:
		LED_Digit_3();
		break;
		case 4:
		LED_Digit_4();
		break;
		case 5:
		LED_Digit_5();
		break;
		case 6:
		LED_Digit_6();
		break;
		case 7:
		LED_Digit_7();
		break;
		case 8:
		LED_Digit_8();
		break;
		case 9:
		LED_Digit_9();
		break;
	}
	_delay_ms(2);
}

void Ones_Display(void) {
	LED_Disp_2();
	switch(countdown % 10) {
		case 0:
		LED_Digit_0();
		break;
		case 1:
		LED_Digit_1();
		break;
		case 2:
		LED_Digit_2();
		break;
		case 3:
		LED_Digit_3();
		break;
		case 4:
		LED_Digit_4();
		break;
		case 5:
		LED_Digit_5();
		break;
		case 6:
		LED_Digit_6();
		break;
		case 7:
		LED_Digit_7();
		break;
		case 8:
		LED_Digit_8();
		break;
		case 9:
		LED_Digit_9();
		break;
	}
	_delay_ms(2);
}

int main(void) {
	Mega328P_Init();	//Initializes the board
	FX_Init();	//Sets up the output pins to power the 4-digit 7-segment display and buzzer
	Countdown_Interrupt_Init();	//Sets up the interrupts for the countdown
	PORTC &= ~(1<<0);	//Clears PC0
	DDRC &= ~(1<<0); //Sets PC0 as input

	while(PINC & (1<<0)) {} //waits for start signal from other board

	sei();	//Enables the interrupts for the 4-digit 7-segment display and buzzer countdown
	while(1) {
	}
}