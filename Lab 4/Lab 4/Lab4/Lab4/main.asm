 ; Lab4
 ;
 ; Created: 3/24/2018 4:15:16 AM
 ; Author : Eugene Rockey

		 .org 0					;student discuss interrupts and vector table in report
		 jmp RESET				;student discuss reset in report
		 jmp INT0_H				;student discuss reset in report
		 jmp INT1_H				;student discuss reset in report
		 jmp PCINT0_H			;student discuss reset in report
		 jmp PCINT1_H			;student discuss reset in report
		 jmp PCINT2_H			;student discuss reset in report
		 jmp WDT				;student discuss reset in report
		 jmp TIM2_COMPA			;student discuss reset in report
		 jmp TIM2_COMPB			;student discuss reset in report
		 jmp TIM2_OVF			;student discuss reset in report
		 jmp TIM1_CAPT			;student discuss reset in report
		 jmp TIM1_COMPA			;student discuss reset in report
		 jmp TIM1_COMPB			;student discuss reset in report
		 jmp TIM1_OVF			;student discuss reset in report
		 jmp TIM0_COMPA			;student discuss reset in report
		 jmp TIM0_COMPB			;student discuss reset in report
		 jmp TIM0_OVF			;student discuss reset in report
		 jmp SPI_TC				;student discuss reset in report
		 jmp USART_RXC			;student discuss reset in report
		 jmp USART_UDRE			;student discuss reset in report
		 jmp USART_TXC			;student discuss reset in report
		 jmp ADCC				;student discuss reset in report
		 jmp EE_READY			;student discuss reset in report
		 jmp ANA_COMP			;student discuss reset in report
		 jmp TWI				;student discuss reset in report
		 jmp SPM_READY			;student discuss reset in report



RESET:	;Initialize the ATMega328P chip for the THIS embedded application.
		;initialize PORTB for Output
		cli
		ldi	r16,0xFF				;PB1 or OC1A Output
		out	DDRB,r16
;initialize and start Timer A, compare match, interrupt enabled
		ldi	r16,0xC0			;set OC to compare match set output to high level
		sts TCCR1A,r16			;student comment here
		ldi r16,0x04			;set clock prescaler
		sts TCCR1B,r16			;student comment here
		ldi r16,0x80			;force output compare, set PB1 high
		sts TCCR1C,r16			;student comment here
		ldi r16,0x40			;Student comment here
		sts TCCR1A,r16			;student comment here
		ldi	r18,0x0B			;student comment here
		ldi r17,0xB8			;student comment here
		lds r16,TCNT1L			;student comment here
		add r17,r16				;student comment here
		lds r16,TCNT1H			;student comment here
		adc r18,r16				;student comment here
		sts OCR1AH,r18			;student comment here
		sts OCR1AL,r17			;student comment here
		ldi r19,0				;student comment here
		ldi r16,0x02			;student comment here
		sts TIMSK1,r16			;student comment here
		out TIFR1,r16			;student comment here
		sei						;student comment here
here:	rjmp here
		
INT0_H:
		nop			;external interrupt 0 handler
		reti
INT1_H:
		nop			;external interrupt 1 handler
		reti
PCINT0_H:
		nop			;pin change interrupt 0 handler
		reti
PCINT1_H:
		nop			;pin change interrupt 1 handler
		reti
PCINT2_H:
		nop			;pin change interrupt 2 handler
		reti
WDT:
		nop			;watch dog time out handler
		reti
TIM2_COMPA:
		nop			;TC 2 compare match A handler
		reti
TIM2_COMPB:
		nop			;TC 2 compare match B handler
		reti
TIM2_OVF:
		nop			;TC 2 overflow handler
		reti
TIM1_CAPT:
		nop			;TC 1 capture event handler
		reti
TIM1_COMPA:			;TC 1 compare match A handler
		sbrc	r19,0				;student comment here
		rjmp	ONE					;student cmment here
		ldi		r17,0xE8			;student comment here
		ldi		r18,0x0B			;student comment here
		ldi		r19,1				;student comment here
		rjmp	BEGIN				;student comment here	
ONE:	ldi		r17,0xE8			;student comment here
		ldi		r18,0x0B			;student comment here
		ldi		r19,0				;student comment here
BEGIN:	lds		r16,OCR1AL			;student comment here
		add		r17,r16				;student comment here
		lds		r16,OCR1AH			;student comment here
		adc		r18,r16				;student comment here
		sts		OCR1AH,r18			;student comment here
		sts		OCR1AL,r17			;student comment here
		reti						;student comment here
TIM1_COMPB:
		nop			;TC 1 compare match B handler
		reti
TIM1_OVF:
		nop			;TC 1 overflow handler
		reti
TIM0_COMPA:
		nop			;TC 0 compare match A handler
		reti
TIM0_COMPB:			
		nop			;TC 1 compare match B handler
		reti
TIM0_OVF:
		nop			;TC 0 overflow handler
		reti
SPI_TC:
		nop			;SPI Transfer Complete
		reti
USART_RXC:
		nop			;USART receive complete
		reti
USART_UDRE:
		nop			;USART data register empty
		reti
USART_TXC:
		nop			;USART transmit complete
		reti
ADCC:
		nop			;ADC conversion complete
		reti
EE_READY:
		nop			;EEPROM ready
		reti
ANA_COMP:
		nop			;Analog Comparison complete 
		reti
TWI:
		nop			;I2C interrupt handler
		reti
SPM_READY:
		nop			;store program memory ready handler
		reti		

