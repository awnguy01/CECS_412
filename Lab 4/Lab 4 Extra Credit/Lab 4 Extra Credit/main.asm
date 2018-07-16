 ; Lab4
 ;
 ; Created: 3/24/2018 4:15:16 AM
 ; Author : Eugene Rockey
 ; Four is Better Than Five
	
	.equ	ADCSRA,0x7A					//student comment here
	.equ	ADSC,6						//student comment here
	.equ	ADCL,0x78					//student comment here
	.equ	ADCH,0x79					//student comment here

    .org  0            ;student discuss interrupts and vector table in report
    jmp   RESET        ;student discuss reset in report
    jmp   INT0_H       ;student discuss reset in report
    jmp   INT1_H       ;student discuss reset in report
    jmp   PCINT0_H     ;student discuss reset in report
    jmp   PCINT1_H     ;student discuss reset in report
    jmp   PCINT2_H     ;student discuss reset in report
    jmp   WDT          ;student discuss reset in report
    jmp   TIM2_COMPA   ;student discuss reset in report
    jmp   TIM2_COMPB   ;student discuss reset in report
    jmp   TIM2_OVF     ;student discuss reset in report
    jmp   TIM1_CAPT    ;student discuss reset in report
    jmp   TIM1_COMPA   ;student discuss reset in report
    jmp   TIM1_COMPB   ;student discuss reset in report
    jmp   TIM1_OVF     ;student discuss reset in report
    jmp   TIM0_COMPA   ;student discuss reset in report
    jmp   TIM0_COMPB   ;student discuss reset in report
    jmp   TIM0_OVF     ;student discuss reset in report
    jmp   SPI_TC       ;student discuss reset in report
    jmp   USART_RXC    ;student discuss reset in report
    jmp   USART_UDRE   ;student discuss reset in report
    jmp   USART_TXC    ;student discuss reset in report
    jmp   ADCC         ;student discuss reset in report
    jmp   EE_READY     ;student discuss reset in report
    jmp   ANA_COMP     ;student discuss reset in report
    jmp   TWI          ;student discuss reset in report
    jmp   SPM_READY    ;student discuss reset in report

RESET:	               ;Initialize the ATMega328P chip for the THIS embedded application initialize PORTB for Output
    cli
    ldi   r16,0xFF     ;PB1 or OC1A Output
    out   DDRB,r16     ;initialize and start Timer A, compare match, interrupt enabled
    ldi   r16,0xC0     ;set OC to compare match set output to high level
    sts   TCCR1A,r16   ;COM1A bits set to high, OC1A set on compare/match
    ldi   r16,0x04     ;set clock prescaler --- change this. use 0x04?
    sts   TCCR1B,r16   ;clock divided by 256
    ldi   r16,0x80     ;force output compare, set PB1 high
    sts   TCCR1C,r16   ;force output compare set for channel A
    ldi   r16,0x40     ;toggle OC1, select PWM mode --- change this. use 0x83? for fast mode?
    sts   TCCR1A,r16   ;OC1B now set to compare/match 
    ldi	  r18,0x0B     ;00001011 loaded to r18 --- change this. use 0x00?
    ldi   r17,0xB8     ;10111000 loaded to r17 --- change this. use 0x00?
    lds   r16,TCNT1L   ;timer counter low byte set to 01000000
    add   r17,r16      ;11111000 value stored in r17
    lds   r16,TCNT1H   ;timer counter high byte also set to 01000000
    adc   r18,r16      ;adds register r18 and r16 for 01001011
    sts   OCR1AH,r18   ;stores high byte of register used to compare with counter
    sts   OCR1AL,r17   ;stores low byte of register used to compare with counter
    ldi   r19,0x00     ;clears r19
    ldi   r16,0x02     ;stores 00000010 in r16 which sets the amount to increment by
    sts   TIMSK1,r16   ;enables the interrupt for output compare A
    out   TIFR1,r16    ;clears the OCF1A flag
    ldi   r16,0x01     ;sets amount to increment by
    ldi   r19,0x00     ;clears r19 --- do we need this a second time?
    ldi   r20,0x01     ;sets r20 to 1 so it can be compared with the increment flag, r22
    ldi   r21,0xFF     ;sets r21 to 255 so it can be compared with the low bit, r17
    ldi   r22,0x00     ;sets r22 to 0. this register will be used as a 'flag' that can be compared to determin increment or decrement
    ldi   r23,0x03     ;sets r23 to 3 to compare with the high bit
    sei                ;sets global interrupt flag

here:
    rjmp  here
    
INT0_H:
    nop                ;external interrupt 0 handler 
    reti

INT1_H: 
    nop                ;external interrupt 1 handler
    reti 

PCINT0_H:
    nop                ;pin change interrupt 0 handler  
    reti

PCINT1_H:
    nop                ;pin change interrupt 1 handler
    reti

PCINT2_H:
    nop                ;pin change interrupt 2 handler
    reti

WDT:
    nop                ;watch dog time out handler
    reti

TIM2_COMPA:
    nop                ;TC 2 compare match A handler
    reti

TIM2_COMPB:
    nop                ;TC 2 compare match B handler
    reti

TIM2_OVF:
    nop                ;TC 2 overflow handler
    reti

TIM1_CAPT:
    nop                ;TC 1 capture event handler
    reti

TIM1_COMPA:            ;TC 1 compare match A handler
ADC_Get:
		ldi		r16,0xC7			//student comment here
		sts		ADCSRA,r16			//student comment here
A2V1:	lds		r16,ADCSRA			//student comment here
		sbrc	r16,ADSC			//student comment here
		rjmp 	A2V1				//student comment here
		lds		r16,ADCL			//student comment here
		lds		r16,ADCH			//student comment here
		ret							//student comment here
	sbrc  r19,0        ;if bit 0 in r19 is cleared then skip the following rjmp to ONE else jump to ONE
    rjmp  ONE          ;if bit 0 was not set in r19 then we need to set it so rel jump to ONE
    ;ldi   r17,0xE8     ;load 0xE8 to r17. 0xE8 = 232 = 11101000. this is the low bit of the output compare --- what is this?
    ;ldi   r18,0x0B     ;load 0x0B to r18. 0xEB = 235 = 11101011. this is the high bit of the output compare --- what is this?
    ;ldi   r19,1        ;load 1 to r19. this sets the bit to 1 --- what is this?
    rjmp  BEGIN         ;rel jump to BEGIN

BEGIN:
    ;lds    r16,OCR1AL  ;load OCR1AL bit to r16. OCR1AL is the output compare register 1A low bit --- what is this?
    ;add    r17,r16     ;add r16 to r17 --- what is this?
    ;lds    r16,OCR1AH  ;load OCR1AH bit to r17. OCR1AH is the output compare register 1A high bit --- what is this?
    ;adc    r18,r16     ;add with carry r16 to r18
    sts   OCR1AH,r18   ;now store the value in r18 to OCR1AH
    sts   OCR1AL,r17   ;now store the value in r17 to OCR1AL
    cp    r22,r19      ;if increment flag, r22, is set to 1 and r19 is 1 then we need to increment
    breq  INCREMENT    ;branch to increment if equal
    cp    r22,r20      ;if increment flag, r22, is set to 0 and r20 is 0 then we need to decrement
    breq  DECREMENT    ;branch to decrement if equal
    reti               ;interrupt return

ONE:
    ldi   r17,0xE8     ;load 0xE8 = 232 = 11101000 directly to r17 --- do we need this?
    ldi   r18,0x0B     ;load 0x0B = 235 = 11101011 directly to r18 --- do we need this?
    ldi   r19,0x00     ;load 0 directly to r19

INCREMENT:
    add   r17,r16      ;increment the low bit of compare register A, r17, by 1 
    adc   r18,r19      ;if there is a carry then add it to the high bit of compare register A, r18
    cp    r18,r23      ;compare high bit with r23 which is 3
    breq  CHECKIF_INC  ;if it is equal then branch to CHECKIF_INC
    reti               ;return from interrupt
    
DECREMENT:
    sub   r17,r16      ;decrement the low bit of compare register A, r17, by 1
    sbc   r18,r19      ;if there is a carry then subtract it from the high bit of compare register A, r18
    cp    r18,r19      ;compare the high bit with 0
    breq  CHECKIF_DEC  ;if it is equal then branch to CHECKIF_DEC
    reti               ;return from interrupt

CHECKIF_INC:
    cpse  r17,r21      ;compare the low bit, r17, with r21, 255, skip if equal
    ldi   r22,0x01     ;set the value of the increment flag to 1 indicating we need to decrement
    reti               ;return from interrupt

CHECKIF_DEC:
    cpse  r17,r19      ;compare the low bit, r17, with r19, 0, skip if equal
    ldi   r22,0x00     ;set the value of the increment flag to 0 indicating we need to increment
    reti               ;return from interrupt

TIM1_COMPB:
    nop                ;TC 1 compare match B handler
    reti

TIM1_OVF:
    nop                ;TC 1 overflow handler
    reti

TIM0_COMPA:
    nop                ;TC 0 compare match A handler
    reti

TIM0_COMPB:
    nop                ;TC 1 compare match B handler
    reti

TIM0_OVF:
    nop                ;TC 0 overflow handler
    reti

SPI_TC:
    nop                ;SPI Transfer Complete
    reti

USART_RXC:
    nop                ;USART receive complete
    reti

USART_UDRE:
    nop                ;USART data register empty
    reti

USART_TXC:
    nop                ;USART transmit complete
    reti

ADCC:
    nop                ;ADC conversion complete
    reti

EE_READY:
    nop                ;EEPROM ready
    reti

ANA_COMP:
    nop                ;Analog Comparison complete 
    reti

TWI:
    nop                ;I2C interrupt handler
    reti

SPM_READY:
    nop                ;store program memory ready handler
    reti