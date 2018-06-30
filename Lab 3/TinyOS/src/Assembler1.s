 // Lab3P1.s
 //
 // Created: 1/30/2018 4:15:16 AM
 // Author : Eugene Rockey
 // Copyright 2018, All Rights Reserved


.section ".data"			;start of the data section
.equ	DDRB,0x04				;DDRB (Data Direction Register B) set to 0x04 (00000100) which 
                      ; sets pin 2 as an output
.equ	DDRD,0x0A				;DDRD (Data Direction Register D) set to 0x0A (00001010) which
                      ; sets pins 1, 3 to outputs
.equ	PORTB,0x05			;sets port B to 0x05 (00000101) which determines if it is high or low
.equ	PORTD,0x0B			;sets port D to 0x0B (00001011) which sets pins 3, 1, and 0 high
.equ	U2X0,1					//student comment here
.equ	UBRR0L,0xC4			//student comment here
.equ	UBRR0H,0xC5			//student comment here
.equ	UCSR0A,0xC0			//student comment here
.equ	UCSR0B,0xC1			//student comment here
.equ	UCSR0C,0xC2			//student comment here
.equ	UDR0,0xC6				//student comment here
.equ	RXC0,0x07				;receive "complete" flag
.equ	UDRE0,0x05			//student comment here
.equ	ADCSRA,0x7A			//student comment here
.equ	ADMUX,0x7C			//student comment here
.equ	ADCSRB,0x7B			//student comment here
.equ	DIDR0,0x7E			//student comment here
.equ	DIDR1,0x7F			//student comment here
.equ	ADSC,6					//student comment here
.equ	ADIF,4					//student comment here
.equ	ADCL,0x78				//student comment here
.equ	ADCH,0x79				//student comment here
.equ	EECR,0x1F				;EECR (EEPROM Control Register) which is used to control the operation 
                      ; of the EEPROM
.equ	EEDR,0x20				;EEDR (EEPROM Data Register) which contains the data that is to be stored
.equ	EEARL,0x21			;EEARL (EEPROM Address Register Low) which is the address of the low byte
.equ	EEARH,0x22			;EEARH (EEPROM Address Register High) which is the address of the high byte
.equ	EERE,0					;EERE (EEPROM Read Enable) which is used to read from the EEPROM
.equ	EEPE,1					;EEPR (EEPROM Write Enable) which is used to write to the EEPROM - this 
                      ; must be 1 otherwise no writing will occur
.equ	EEMPE,2					;EEMPE (EEPROM Master Write Enable) which is used to determine if EEPE 
                      ; causes the EEPROM to be written to
.equ	EERIE,3					;EERIE (EEPROM Ready Interrupt Enable) which enables the EEPROM ready
                      ; interrupt if the I bit in the SREG is set

.global HADC				  //global variable intended to hold the high byte of ADC
.global LADC				  //global variable intended to hold the low byte of ADC
.global ASCII				  //global variable intended to hold a character byte
.global DATA				  //global variable intended to hold any data byte
.global hAddress      //high bit used for eeprom address
.global lAddress      //low bit used for eeprom address
.global	content       //content storage variable used for eeprom

.set	temp,0				  //student comment here

.section ".text"			//student comment here
.global Mega328P_Init
Mega328P_Init:
		ldi	r16,0x07		  ;PB0(R*W),PB1(RS),PB2(E) as fixed outputs
		out	DDRB,r16		  //student comment here
		ldi	r16,0			    //student comment here
		out	PORTB,r16		  //student comment here
		out	U2X0,r16	   	;initialize UART, 8bits, no parity, 1 stop, 9600
		ldi	r17,0x0		  	//storing the value of the high byte for 9600 Baud rate
		ldi	r16,0x67		  //storing the value of the low byte for 9600 Baud rate
		sts	UBRR0H,r17		//setting the high byte for Baud rate
		sts	UBRR0L,r16		//setting the low byte for Baud rate
		ldi	r16,24		  	//value to enable the receiver and transmitter
		sts	UCSR0B,r16		//enabling the receiver and transmitter
		ldi	r16,6			    //student comment here
		sts	UCSR0C,r16		//student comment here
		ldi r16,0x87		  //initialize ADC
		sts	ADCSRA,r16		//student comment here
		ldi r16,0x40		  //student comment here
		sts ADMUX,r16		  //student comment here
		ldi r16,0			    //student comment here
		sts ADCSRB,r16		//student comment here
		ldi r16,0xFE		  //student comment here
		sts DIDR0,r16	  	//student comment here
		ldi r16,0xFF	  	//student comment here
		sts DIDR1,r16	  	//student comment here
		ret				      	;return from subroutine

.global BAUD_Byte_Set
BAUD_Byte_Set:
	

.global LCD_Write_Command
LCD_Write_Command:
	call	UART_Off		  //student comment here
	ldi		r16,0xFF		  ;PD0 - PD7 as outputs
	out		DDRD,r16		  //student comment here
	lds		r16,DATA		  //student comment here
	out		PORTD,r16		  //student comment here
	ldi		r16,4			    //student comment here
	out		PORTB,r16		  //student comment here
	call	LCD_Delay		  //student comment here
	ldi		r16,0			    //student comment here
	out		PORTB,r16		  //student comment here
	call	LCD_Delay		  //student comment here
	call	UART_On			  //student comment here
	ret						      //student comment here

LCD_Delay:
	ldi		r16,0xFA		  //student comment here
D0:	ldi		r17,0xFF	  //student comment here
D1:	dec		r17				  //student comment here
	brne	D1				    //student comment here
	dec		r16			  	  //student comment here
	brne	D0			  	  //student comment here
	ret						      //student comment here

.global LCD_Write_Data
LCD_Write_Data:
	call	UART_Off		  //student comment here
	ldi		r16,0xFF		  //student comment here
	out		DDRD,r16		  //student comment here
	lds		r16,DATA		  //student comment here
	out		PORTD,r16		  //student comment here
	ldi		r16,6			    //student comment here
	out		PORTB,r16		  //student comment here
	call	LCD_Delay		  //student comment here
	ldi		r16,0			    //student comment here
	out		PORTB,r16		  //student comment here
	call	LCD_Delay		  //student comment here
	call	UART_On			  //student comment here
	ret						      //student comment here

.global LCD_Read_Data
LCD_Read_Data:
	call	UART_Off		  //student comment here
	ldi		r16,0x00		  //student comment here
	out		DDRD,r16		  //student comment here
	out		PORTB,4			  //student comment here
	in		r16,PORTD		  //student comment here
	sts		DATA,r16		  //student comment here
	out		PORTB,0			  //student comment here
	call	UART_On			  //student comment here
	ret						      //student comment here

.global UART_On
UART_On:
	ldi		r16,2				  ;load 2 into r16
	out		DDRD,r16		  ;sets direction of port d to 2
	ldi		r16,24			  ;loads 24 into r16
	sts		UCSR0B,r16	  ;store r16 content into USCR0B
	ret							    ;return from subroutine

.global UART_Off
UART_Off:
	ldi	r16,0					  ;loads 0 (or low) into r16
	sts UCSR0B,r16			;loads contents of r16 into UCSR0B in other words sets 
                      ; UCSR0B to low which turns off UART
	ret							    ;return from subroutine

.global UART_Clear
UART_Clear:
	lds		r16,UCSR0A		;load UCSR0A into r16
	sbrs	r16,RXC0			;skip the following return instruction if RXC0 bit is set in r16
	ret							    ;return from subroutine when RXC0 is not set
	lds		r16,UDR0			;if previous return did not occur load UDR0 into r16 and try again
	rjmp	UART_Clear		;loop over UART_Clear

.global UART_Get
UART_Get:
	lds		r16,UCSR0A		;retrieves the status register that checks if there is data in the buffer
	sbrs	r16,RXC0			;skips the next instruction if there is data in the buffer
	rjmp	UART_Get			;loops the data retrieval function until there is data in the buffer
	lds		r16,UDR0			;stores the data in the buffer in r16
	sts		ASCII,r16			;stores the register data into the address pointed to by ASCII
	ret							    ;return from subroutine

.global UART_Put
UART_Put:
	lds		r17,UCSR0A	  ;store USCR0A (control status register A) into r17
	sbrs	r17,UDRE0		  ;if the UART data register empty bit is set then skip the following
                      ; relative jump instruction
	rjmp	UART_Put		  ;if the UART data register empty bit is not set loop back
                      ; through UART_Put 
	lds		r16,ASCII		  ;load what is stored in ASCII to r16
	sts		UDR0,r16		  ;store value in r16 to UDR0
	ret							    ;return from subroutine

.global ADC_Get
ADC_Get:
		ldi		r16,0xC7	    ;load 0xC7 (RXC0) into r16
		sts		ADCSRA,r16	  ;store from r16 into ADCSRA (responsible for enable ADC and
                        ; start ADC) to enable ADC
A2V1:	lds		r16,ADCSRA  ;load ADCSRA into r16
		sbrc	r16,ADSC		  ;check if the start conversion bit has been set on r16, if so 
                        ; skip otherwise continue to the followign instruction
		rjmp 	A2V1				  ;if the start conversion has not been set then loop back to A2V1
		lds		r16,ADCL		  ;load from ADCL (ADC Low) into r16
		sts		LADC,r16		  ;store what is now stored in r16 to LADC (global variable to 
                        ; hold the low byte of ADC) 
		lds		r16,ADCH		  ;load from ADCH (ADC How) into r16
		sts		HADC,r16		  ;store what is now stored in r16 to HADC (global variable to 
                        ; hold the high byte of ADC)
		ret							    ;return from subroutine

.global EEPROM_Write
EEPROM_Write:      
		sbic    EECR,EEPE
		rjmp    EEPROM_Write		;Wait for completion of previous write
		lds		  r18,lAddress	  ;Set up address (r18:r17) in address register
		lds		  r17,hAddress
		lds	  	r16,content			;Set up data in r16    
		out     EEARH, r18      
		out     EEARL, r17			      
		out     EEDR,r16		  	;Write data (r16) to Data Register  
		sbi     EECR,EEMPE			;Write logical one to EEMPE
		sbi     EECR,EEPE			  ;Start eeprom write by setting EEPE
		ret                     ;return from subroutine

.global EEPROM_Read
EEPROM_Read:					    
		sbic    EECR,EEPE    
		rjmp    EEPROM_Read	  	;Wait for completion of previous write
		lds		  r18,lAddress		;Set up address (r18:r17) in EEPROM address register
		lds		  r17,hAddress
		lds		  r16,content
		out     EEARH, r18   
		out     EEARL, r17		   
		sbi     EECR,EERE		    ;Start eeprom read by writing EERE
		in      r16,EEDR	  	  ;Read data from Data Register
		sts		  ASCII,r16  
		ret                     ;return from subroutine

		.end

