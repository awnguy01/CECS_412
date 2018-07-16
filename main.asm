 ;
 ;
 ;
 ;
 ;

	.org 0			    ;student discuss interrupts and vector table in report
	jmp RESET		    ;student discuss reset in report
	jmp INT0_H		  ;student discuss reset in report
	jmp INT1_H		  ;student discuss reset in report
	jmp PCINT0_H		;student discuss reset in report
	jmp PCINT1_H		;student discuss reset in report
	jmp PCINT2_H		;student discuss reset in report
	jmp WDT		      ;student discuss reset in report
	jmp TIM2_COMPA	;student discuss reset in report
	jmp TIM2_COMPB	;student discuss reset in report
	jmp TIM2_OVF		;student discuss reset in report
	jmp TIM1_CAPT		;student discuss reset in report
	jmp TIM1_COMPA	;student discuss reset in report
	jmp TIM1_COMPB	;student discuss reset in report
	jmp TIM1_OVF		;student discuss reset in report
	jmp TIM0_COMPA	;student discuss reset in report
	jmp TIM0_COMPB	;student discuss reset in report
	jmp TIM0_OVF		;student discuss reset in report
	jmp SPI_TC		  ;student discuss reset in report
	jmp USART_RXC		;student discuss reset in report
	jmp USART_UDRE	;student discuss reset in report
	jmp USART_TXC		;student discuss reset in report
	jmp ADCC		    ;student discuss reset in report
	jmp EE_READY		;student discuss reset in report
	jmp ANA_COMP		;student discuss reset in report
	jmp TWI		      ;student discuss reset in report
	jmp SPM_READY		;student discuss reset in report

RESET:	            ;Initialize the ATMega328P chip for the THIS embedded application initialize PORTB for Output
	  cli
	  ldi	r16,0xFF	  ;PB1 or OC1A Output -----
	  out	DDRB,r16	  ;Initialize and start Timer A, compare match, interrupt enabled -----
    ldi	r16,0xC0	  ;set OC to compare match set output to high level -----
	  sts TCCR1A,r16	;COM1A bits set to high, OC1A set on compare/match -----
	  ldi r16,0x0B		;Set clock prescaler --------------- we ues 0x04
	  sts TCCR1B,r16	;Writes the value above to TCCR1B to apply the changes -----
	  ldi r16,0x80		;Force output compare, set PB1 high -----
	  sts TCCR1C,r16	;Writes the value above to TCCR1C to apply the changes -----
	  ldi r16,0x83		;Select the Fast PWM Mode 7 ----- we use 0x40
	  sts TCCR1A,r16	;Writes the value above to TCCR1A to apply the changes -----
	  ldi	r18,0x00	  ;Set the low bit to the value of 0 --------------- we use 0x0B
	  ldi r17,0x00		;Set the high bit to the value of 0 --------------- we use 0xB8
	  lds r16,TCNT1L	;student comment here ----- 
	  add r17,r16		  ;student comment here -----
	  lds r16,TCNT1H	;student comment here -----
	  adc r18,r16		  ;student comment here -----
	  sts OCR1AH,r18	;student comment here -----
	  sts OCR1AL,r17	;student comment here -----
	  ldi r19,0x00		;student comment here -----
	  ldi r16,0x02		;student comment here -----
	  sts TIMSK1,r16	;student comment here -----
	  out TIFR1,r16		;student comment here -----
	  ldi r16,0x01		;Set the amount to increment by --------------- we use 0x01
	  clr r19		      ;Set the register to 0 to compare against the incre3ase/decrease flag, the low bit and the high bit when decreasing ---------------
	  ldi r20,0x01		;Set the register to 1 to compare against the increase/decrease flag -----
	  ldi r21,0xFF		;Set the register to 255 to compare against the low bit -----
	  clr r22		      ;Set the register to 0 to act as flag to determine if the screen should increase or decrease brightness --------------- we use ldi r22, 0x00
	  ldi r23,0x03		;Set the register to 3 to compare against the high bit --------------- yes but what is this
    sei			        ;Sets the global interrupt flag ----
here:	; -----
  rjmp here		      ;Loop until interrupted -----
		
INT0_H: ; -----
	nop			          ;external interrupt 0 handler -----
	reti ; -----
INT1_H: ; -----
	nop			          ;external interrupt 1 handler -----
	reti            ; -----
PCINT0_H:         ; -----
	nop			        ;pin change interrupt 0 handler -----
	reti            ; -----
PCINT1_H:         ; -----
	nop			        ;pin change interrupt 1 handler -----
	reti            ; -----
PCINT2_H:         ; -----
	nop			        ;pin change interrupt 2 handler -----
	reti            ; -----
WDT:              ; -----
	nop			        ;watch dog time out handler -----
	reti            ; -----
TIM2_COMPA:       ; -----
	nop			        ;TC 2 compare match A handler -----
	reti            ; -----
TIM2_COMPB:       ; -----
	nop		          ;TC 2 compare match B handler -----
	reti            ; -----
TIM2_OVF:         ; -----
	nop			        ;TC 2 overflow handler -----
	reti            ; -----
TIM1_CAPT:        ; -----
	nop			        ;TC 1 capture event handler
	reti
TIM1_COMPA:			  ;TC 1 compare match A handler -----
BEGIN:	
  sts OCR1AH,r18	; Store the current value of register 18 into the high bit of compare register A -----
	sts OCR1AL,r17	; Store the current value of register 18 into the high bit of compare register A -----
	cp r22,r19		  ; Compare the increase/decreaase flag with 0 -----
	breq increase		; If its equal jump to increase -----
	cp r22,r20		  ; Compare the increase/decreaase flag with 1 -----
	breq	decrease	; If its equal jump to increase -----
	reti			      ; Return from interrupt -----

increase: 
	add r17,r16		  ; Add the increment value to the low bit value of compare register A -----
	adc r18,r19		  ; If there is a carry, add it to the high bit -----
	cp r18,r23		  ; Compare the high bit to 3 -----
	breq inccheck	  ; If its equal jump to the increase checker -----
	reti			      ; Return from interrupt 

decrease:
	sub r17,r16		  ; Subtract the increment value from the low bit value of compare register A  -----
	sbc r18,r19		  ; If there is a carry, subtract it from the high bit -----
	cp r18,r19		  ; Compare the high bit to 0 -----
	breq deccheck	  ; if its equal jump to the decrease checker -----
	reti			      ; Return from interrupt -----

inccheck:
	cp r17,r21		  ; Compare the low bit to 255 
	breq incswap	  ; If its equal then jump to the increase swap
	reti			      ; Return from interrupt

incswap:
	ldi r22, 0x01	  ; Set the value of the increase/decrease flag to 1 (decrease)
	reti			      ; Return from interrupt

deccheck:
	cp r17,r19		  ; Compare the low bit to 0
	breq decswap	  ; If its equal then jump to the decrease swap
	reti			      ; Return from interrupt

decswap:							
  clr r22		        ; Set the value of the increase/decrease flag to 0 (increase)
	reti			      ; Return from interrupt

TIM1_COMPB:       ; -----
	nop			        ;TC 1 compare match B handler
	reti
TIM1_OVF:
	nop		  	      ;TC 1 overflow handler
	reti
TIM0_COMPA:
	nop			        ;TC 0 compare match A handler
	reti
TIM0_COMPB:			
	nop			        ;TC 1 compare match B handler
	reti
TIM0_OVF:
	nop			        ;TC 0 overflow handler
	reti
SPI_TC:
	nop			        ;SPI Transfer Complete
	reti
USART_RXC:
	nop			        ;USART receive complete
	reti
USART_UDRE:
	nop			        ;USART data register empty
	reti
USART_TXC:
	nop			        ;USART transmit complete
	reti
ADCC:
	nop			        ;ADC conversion complete
	reti
EE_READY:
	nop			        ;EEPROM ready
	reti
ANA_COMP:
	nop 			      ;Analog Comparison complete 
	reti
TWI:
	nop	  		      ;I2C interrupt handler
	reti
SPM_READY:
	nop		  	      ;store program memory ready handler
	reti		