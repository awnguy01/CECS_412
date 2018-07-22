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
.equ	EECR,0x1F				;EECR (EEPROM Control Register) which is used to control the operation 
									;of the EEPROM
.equ	EEDR,0x20				;EEDR (EEPROM Data Register) which contains the data that is to be stored
.equ	EEARL,0x21				;EEARL (EEPROM Address Register Low) which is the address of the low byte
.equ	EEARH,0x22				;EEARH (EEPROM Address Register High) which is the address of the high byte
.equ	EERE,0					;EERE (EEPROM Read Enable) which is used to read from the EEPROM
.equ	EEPE,1					;EEPR (EEPROM Write Enable) which is used to write to the EEPROM - this 
									;must be 1 otherwise no writing will occur
.equ	EEMPE,2					;EEMPE (EEPROM Master Write Enable) which is used to determine if EEPE 
								;causes the EEPROM to be written to
.equ	EERIE,3					;EERIE (EEPROM Ready Interrupt Enable) which enables the EEPROM ready
								;interrupt if the I bit in the SREG is set

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
	

.global Detect_Press
Detect_Press:
	ldi		r16,1
	sbis	PINB,7
	sts		ASCII,r16
	ret

.global LCD_Write_Command
LCD_Write_Command:
	call	UART_Off			;turns off UART
	ldi		r16,0xFF			;PD0 - PD7 as outputs
	out		DDRD,r16			;sets pins 0-7 as outputs
	lds		r16,DATA			;loads the contents of the DATA variable that contain the LCD command
	out		PORTD,r16			;outputs the contents of the DATA variable to port D
	ldi		r16,4			    ;sets the output to not display on the LCD
	out		PORTB,r16			;output does not display on the LCD
	call	LCD_Delay			;delays the program
	ldi		r16,0			    ;clears the data in port B
	out		PORTB,r16			;port B cleared
	call	LCD_Delay			;delays the program
	call	UART_On				;re-enables UART
	ret						    ;return from subroutine

LCD_Delay:
	ldi		r16,0xFA			;sets a high value for the delay
D0:	ldi		r17,0xFF			;sets a high value for the delay
D1:	dec		r17					;decrements r17 continuously until 0 to create delay
	brne	D1				    ;loops back to decrement r17 until 0
	dec		r16			  		;decrements r16 continuously until 0 to create delay
	brne	D0			  		;loops back to decrement r16 until 0
	ret						    ;return from subroutine

.global LCD_Write_Data
LCD_Write_Data:
	call	UART_Off			;turns off UART
	ldi		r16,0xFF			;PD0 - PD7 as outputs
	out		DDRD,r16			;sets pins 0-7 as outputs
	lds		r16,DATA			;loads the contents of the DATA variable that contain the output
	out		PORTD,r16			;outputs the contents of the DATA variable to port D
	ldi		r16,6			    ;sets the output needed for the LCD to output to the screen
	out		PORTB,r16			;registers that the data from port D is to be outputted to the screen
	call	LCD_Delay			;delays the program
	ldi		r16,0			    ;clears the data in port B
	out		PORTB,r16			;port B cleared
	call	LCD_Delay			;delays the program
	call	UART_On				;re-enables UART
	ret						    ;return from subroutine

.global LCD_Read_Data
LCD_Read_Data:
	call	UART_Off			;turns off UART
	ldi		r16,0x00			;PD0 - PD7 as inputs
	out		DDRD,r16			;sets pins 0-7 as inputs
	out		PORTB,4				;LCD does not display anything onto the screen
	in		r16,PORTD			;stores the data in port D to r16
	sts		DATA,r16			;stores that data in the DATA variable
	out		PORTB,0				;clears port B
	call	UART_On				;re-enables UART
	ret						    ;return from subroutine

.global UART_On
UART_On:
	ldi		r16,2				;load 2 into r16
	out		DDRD,r16			;sets direction of port d to 2
	ldi		r16,24				;loads 24 into r16
	sts		UCSR0B,r16			;store r16 content into USCR0B
	ret							;return from subroutine

.global UART_Off
UART_Off:
	ldi	r16,0					;loads 0 (or low) into r16
	sts UCSR0B,r16				;loads contents of r16 into UCSR0B in other words sets 
									;UCSR0B to low which turns off UART
	ret							;return from subroutine

.global UART_Clear
UART_Clear:
	lds		r16,UCSR0A			;load UCSR0A into r16
	sbrs	r16,RXC0			;skip the following return instruction if RXC0 bit is set in r16
	ret							;return from subroutine when RXC0 is not set
	lds		r16,UDR0			;if previous return did not occur load UDR0 into r16 and try again
	rjmp	UART_Clear			;loop over UART_Clear

.global UART_Get
UART_Get:
	lds		r16,UCSR0A			;retrieves the status register that checks if there is data in the buffer
	sbrs	r16,RXC0			;skips the next instruction if there is data in the buffer
	rjmp	UART_Get			;loops the data retrieval function until there is data in the buffer
	lds		r16,UDR0			;stores the data in the buffer in r16
	sts		ASCII,r16			;stores the register data into the address pointed to by ASCII
	ret							;return from subroutine

.global UART_Put
UART_Put:
	lds		r17,UCSR0A			;store USCR0A (control status register A) into r17
	sbrs	r17,UDRE0			;if the UART data register empty bit is set then skip the following
									;relative jump instruction
	rjmp	UART_Put			;if the UART data register empty bit is not set loop back
									;through UART_Put 
	lds		r16,ASCII			;load what is stored in ASCII to r16
	sts		UDR0,r16			;store value in r16 to UDR0
	ret							;return from subroutine
		
.global EEPROM_Write
EEPROM_Write:      
		sbic    EECR,EEPE
		rjmp    EEPROM_Write	;Wait for completion of previous write
		ldi		r18,0x05			;Set up address (r18:r17) in address register
		ldi		r17,0x00
		lds	  	r16,ASCII		;Set up data in r16    
		out     EEARH, r18      
		out     EEARL, r17			      
		out     EEDR,r16		;Write data (r16) to Data Register  
		sbi     EECR,EEMPE		;Write logical one to EEMPE
		sbi     EECR,EEPE		;Start eeprom write by setting EEPE
		ret                     ;return from subroutine

.global EEPROM_Read
EEPROM_Read:					    
		sbic    EECR,EEPE    
		rjmp    EEPROM_Read	  	;Wait for completion of previous write
		ldi		r18,5	;Set up address (r18:r17) in EEPROM address register
		ldi		r17,0
		out     EEARH, r18   
		out     EEARL, r17		   
		sbi     EECR,EERE		;Start eeprom read by writing EERE
		in      r16,EEDR	  	;Read data from Data Register
		sts		ASCII,r16  
		ret                     ;return from subroutine
		
		.end

