 // Lab3P1.s
 //
 // Created: 1/30/2018 4:15:16 AM
 // Author : Eugene Rockey
 // Copyright 2018, All Rights Reserved


.section ".data"				;start of the data section
.equ	PINB,0X03
.equ	DDRB,0x04				;DDRB (Data Direction Register B) set to 0x04 (00000100) which 
									;sets pin 2 as an output
.equ	DDRD,0x0A				;DDRD (Data Direction Register D) set to 0x0A (00001010) which
									;sets pins 1, 3 to outputs
.equ	PORTB,0x05				;sets port B to 0x05 (00000101) which determines if it is high or low
.equ	PORTD,0x0B				;sets port D to 0x0B (00001011) which sets pins 3, 1, and 0 high
.equ	U2X0,1					;Doubles the USART transmission speed
.equ	UBRR0L,0xC4				;specifies the address offset of the low byte of the baud rate register
.equ	UBRR0H,0xC5				;specifies the address offset of the high byte of the baud rate register
.equ	UCSR0A,0xC0				;specifies the address offset of the USART control and status register A
.equ	UCSR0B,0xC1				;specifies the address offset of the USART control and status register B
.equ	UCSR0C,0xC2				;specifies the address offset of the USART control and status register C
.equ	UDR0,0xC6				;specifies the address offset of the USART data register
.equ	RXC0,0x07				;receive "complete" flag
.equ	UDRE0,0x05				;data register "empty" flag
.equ	DIDR0,0x7E				;specifies the address offset of the digital input disable register 0
.equ	DIDR1,0x7F				;specifies the address offset of the digital input disable register 1

.global ASCII					;global variable intended to hold a character byte
.global DATA					;global variable intended to hold any data byte
.set	temp,0					;sets the temp assembly variable to 0

.section ".text"				;start of the text section
.global Mega328P_Init
Mega328P_Init:
		ldi	r16,0x07			;PB0(R*W),PB1(RS),PB2(E) as fixed outputs
		out	DDRB,r16			;Sets Pins 0, 1, and 2 as outputs
		ldi	r16,0			    ;R16 cleared to put out to port B
		out	PORTB,r16			;Clears any data in I/O port B
		out	U2X0,r16	   		;initialize UART, 8bits, no parity, 1 stop, 9600
		ldi	r17,0x0		  		;storing the value of the high byte for 9600 Baud rate
		ldi	r16,0x67			;storing the value of the low byte for 9600 Baud rate
		sts	UBRR0H,r17			;setting the high byte for Baud rate
		sts	UBRR0L,r16			;setting the low byte for Baud rate
		ldi	r16,24		  		;value to enable the receiver and transmitter
		sts	UCSR0B,r16			;enabling the receiver and transmitter
		ldi	r16,6			    ;storing the value to set the character size to 8-bits
		sts	UCSR0C,r16			;setting the character size to 8-bits
		ldi r16,0xFE			;sets which ADC pins have their digital input buffer disabled
		sts DIDR0,r16	  		;disables all ADC pins except for ADC0D
		ldi r16,0xFF	  		;determines which AINx pins to disable
		sts DIDR1,r16	  		;disables both AINx pins
		ret				      	;return from subroutine
			
		.end

