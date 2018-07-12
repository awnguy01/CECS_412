/*  
* lab2p2.asm  
* Celsius to Fahrenheit Look-Up Table  
*  Created: 6/2/2014 10:17:31 AM  *   Author: Eugene Rockey  */    
		.dseg   
		.org	0x100
output: .byte	1				;reserves one byte of space for the output Fahrenheit value 
		.cseg   
		.org	0x0
		jmp		main			;partial vector table at address 0x0   
		.org	0x100			;MAIN entry point at address 0x200 (step through the code) 
main:  ldi		ZL,low(2*table) ;sets the low byte of Z-register to the low byte address of the data table
		ldi		ZH,high(2*table);sets the high byte of the Z-register to the high byte address of the data table  
		ldi		r16,celsius		;stores the input celsius value into r16
		add		ZL,r16			;adds the value of r16 to Z, which moves the index of the data table that many values
		ldi		r16,0			;clears r16
		adc		ZH,r16			;sets the high byte of the Z-register to 0   
		lpm						;lpm = lpm r0,Z in reality, what does this mean?
								;loads into the default register r0, the value stored at the address now pointed to by the Z-register, which corresponds to an index in the lookup table   
		sts		output,r0		;store look-up result to SRAM
		ret						;consider MAIN as a subroutine to return from - but back to where??
								;returns the program counter to the beginning of the code segment at 0x00  
								;Fahrenheit look-up table 
table:  .db		32, 34, 36, 37, 39, 41, 43, 45, 46, 48, 50, 52, 54, 55, 57, 59, 61, 63, 64, 66
		.equ	celsius = 5		;modify Celsius from 0 to 19 degrees for different results  
		.exit