;
; SortTable_L2.asm
;
; Created: 6/13/2018 12:48:03 AM
; Author : Andrew Nguyen Vo
;   
			.dseg						; directive for specifying data segment SRAM
			.org	0x100				; sets origin of SRAM to be at 0x100
			.cseg						; directive for specifying code segment in FLASH memory
			.org	0x0					; sets origin of FLASH memory to be at 0x0
			jmp		main				; MAIN entry point at address 0x200
main:		ldi		XL,0x00				; Sets low byte of X-register to 0x00
			ldi		XH,0x01				; Sets high byte of X-register to 0x01
			ldi		ZL,low(2*table)		; Sets the low byte of the Z-register to the low byte of the byte address of the lookup table
			ldi		ZH,high(2*table)	; Sets the high byte of the Z-register to the high byte of the byte address of the lookup table
			ldi		r20,0				; Sets the value of r20 to 0
										; Store: used to copy all the lookup table values to SRAM
store:		inc		r20					; increments r20 by 1
			lpm		r16,Z+				; loads into r16 the value pointed to by the Z-register and post-increments Z by 1
			st		X+,r16				; loads into address pointed to by X-register the value of r16 and post-increments the value of X by 1
			cpi		r20,20				; compares the value in r20 with the number 20
			brlo	store				; if the value in r20 is lower than 20, loop back to store:
										; repeat loop 20 times until all 20 numbers from lookup table are copied over to SRAM
			ldi		r20,0				; clears any value in r20
			ldi		XL,0x00				; resets low byte of X-register to 0x00
			ldi		XH,0X01				; resets high byte of X-register to 0x01
			movw	ZL,XL				; copies the byte pair of the X-register to the byte pair of the Z-register
			jmp		sort				; unconditionally jumps to sort:
										; sort: acts as the outermost for-loop, iterating through all the values from the table
sort:		ld		r16,X+				; loads into r16 the value pointed to by the X-register and post-increments X by 1
			mov		r21,r20				; copies the value in r20 to r21
			movw	ZL,XL				; copies the byte pair of the X-register to the Z-register
			inc		r20					; increments r20 by 1
			cpi		r20,20				; compares the value in r20 to the number 20
			brge	done				; if the value in r20 is greater than or equal to 20, then branch to done:
			jmp		compare				; else, unconditionally jump to compare:
										; compare: acts as a nested for-loop to compare each value with every value to its right using min-max insertion sort
compare:	cpi		r21,19				; compare the value in r21 with 19
			brge	sort				; if the value in r21 is greater than or equal to 19, branch to sort:
			inc		r21					; increment the value of r21 by 1
			ld		r17,Z+				; load into r17 the value pointed to by the Z-register
			cp		r17,r16				; compare the values of r17 and r16
			brlo	switch				; if r17 is lower than r16, then branch to switch:
			jmp		compare				; else, loop back to compare:
										; switch: is used to swap the values at different addresses if the right one is lower than the left one
switch:		st		-X,r17				; pre-decrements X to point to the address right before and stores in it the value of r17
			st		-Z,r16				; pre-decrements Z to point to the address right before and stores in it the value of r16
			ld		r16,X+				; loads into r16 the new value pointed to by the X-register and post-increments X by 1
			ld		r17,Z+				; loads into r17 the new value pointed to by the Z-register and post-increments Z by 1
			jmp		compare				; loops back to compare:
	
table:  	.db		52,12,85,45,9,62,3,4,88,1,20,4,13,02,51,02,79,6,5,19
	
done:		jmp		done
			.exit
	/*
			ldi		r16,celsius			;student comment goes here   
			add		ZL,r16				;student comment goes here   
			ldi		r16,0				;student comment goes here   
			adc		ZH,r16				;student comment goes here   
			lpm							;lpm = lpm r0,Z in reality, what does this mean?   
			sts		output,r0			;store look-up result to SRAM   
			ret							;consider MAIN as a subroutine to return from - but back to where??       
										; Fahrenheit look-up table 
			.equ	celsius = 5			;modify Celsius from 0 to 19 degrees for different results   
			.exit

			*/