;
; NestedSubroutines_L2.asm
;
; Created: 6/13/2018 12:45:57 AM
; Author : Andrew Nguyen Vo
; 
 ;******************************  
 ;* Declare Variables  
 ;******************************    
			.dseg					;directive to specify data segment in SRAM
			.org	0x100			;originate data storage at address 0x100  
quotient:	.byte	1				;uninitialized quotient variable stored in SRAM aka data segment  
remainder:	.byte	1				;uninitialized remainder variable stored in SRAM    
			.set	count = 0		;initialized count variable stored in SRAM  
;******************************    
			.cseg					;Declare and Initialize Constants (modify them for different results)    
			.equ	dividend = 13	;8-bit dividend constant (positive integer) stored in FLASH memory aka code segment    
			.equ	divisor = 3		;8-bit divisor constant (positive integer) stored in FLASH memory  
;******************************  
;* Vector Table (partial)  
;******************************   
			.org	0x0				;sets origin of code segment in FLASH memory at 0x0
reset:		jmp		main			;RESET Vector at address 0x0 in FLASH memory (handled by MAIN) 
int0v:		jmp		int0h			;External interrupt vector at address 0x2 in Flash memory (handled by int0)  
;******************************		
;* MAIN entry point to program*  
;******************************    
			.org	0x100			;originate MAIN at address 0x100 in FLASH memory (step through the code)  
main:		call	init			;initialize variables subroutine, set break point here, check the STACK,SP,PC
									;STACK = ?? ??, SP 0x08FF, PC = 0x00000100
			call	getnums			;Check the STACK,SP,PC here
									;STACK = ?? ??, SP = 0x08FF, PC = 0x00000102
			call	test		    ;Check the STACK,SP,PC here
									;STACK = ?? ??, SP = 0x08FF, PC = 0x00000104
			call	divide			;Check the STACK,SP,PC here
									;STACK = ?? ??, SP = 0x08FF, PC = 0x00000106
endmain:	jmp		endmain
init:		lds		r0,count		;get initial count, set break point here and check the STACK,SP,PC   
			sts		quotient,r0		;use the same r0 value to clear the quotient-   
			sts		remainder,r0	;and the remainder storage locations
			ret						;return from subroutine, check the STACK,SP,PC here.
									;STACK = 01 02, SP = 0x08FD, PC = 0x00000110
getnums:	ldi		r30,dividend	;Check the STACK,SP,PC here.
									;STACK = 01 04, SP = 0x08FD, PC = 0x00000111   
			ldi		r31,divisor		;loads r31 with the value specified by the symbol divisor
			ret						;Check the STACK,SP,PC here.
									;STACK = 01 04, SP = 0x08FD, PC = 0x00000113 
test:		cpi		r30,0			; is dividend == 0 ?   
			brne	test2			;if the value in r30 is not 0, branch to test2:
test1:		jmp		test1			; halt program, output = 0 quotient and 0 remainder 
test2:		cpi		r31,0			; is divisor == 0 ?   
			brne	test4			;if the value in r31 is not 0, branch to test4:
			ldi		r30,0xEE		; set output to all EE's = Error division by 0   
			sts		quotient,r30	;stores the value in r30 into the variable specified by quotient
			sts		remainder,r30	;stores the value of r30 into the variable specified by remainder
test3:		jmp		test3			; halt program, look at output 
test4:		cp		r30,r31			; is dividend == divisor ?   
			brne	test6			;if the values in r30 and r31 are not the same, branch to test6:
			ldi		r30,1			;then set output accordingly   
			sts		quotient,r30	;stores the value of r30 in the variable specified by quotient
test5:		jmp		test5			; halt program, look at output 
test6:		brpl	test8			; is dividend < divisor ?   
			ser		r30				; sets all the bits in r30
			sts		quotient,r30	; stores the value of r30 in the variable specified by quotient
			sts		remainder,r30	; set output to all FF's = not solving Fractions in this program 
test7:		jmp		test7			; halt program look at output 
test8:		ret						; otherwise, return to do positive integer division 
divide:		lds		r0,count		;loads r0 with the initial value 0 specified by the count symbol
divide1:	inc		r0				;increments the value of 30 by 1 for every time this subroutine is entered/looped. This is equal to the quotient
			sub		r30,r31			;subtracts the value in r30 by the value in r31 and stores in r30. This is the standard process of division.
			brpl	divide1			;if negative flag is not set by the subtraction, loop back to divide1: in order to get the maximum number of times the value in r31 goes into the value in r30 
			dec		r0				;else decrement r0 by 1 because this implies that the quotient is one less than the current number stored in r0
			add		r30,r31			;add the value in r30 with the value in r31. This should be less than the value in the divisor stored in r31, which is otherwise known as the remainder
			sts		quotient,r0		;stores the value of r0 in the variable specified by quotient
			sts		remainder,r30	;stores the value of r30 in the variable specified by remainder
divide2:	ret						;return from this subroutine back to the main
int0h:		jmp		int0h			; interrupt 0 handler goes here 
			.exit