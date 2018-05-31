;
; project1.asm
;
; Created: 5/29/2018 11:10:34 PM
; Author : Andrew Nguyen Vo
;

			.cseg				;FLASH code segment
			.org	0x0			;sets the origin to 0x0
start:		ldi		r26,25		;loads integer 26 into r26: hex(0x19)
			ldi		r27,21		;loads integer 27 into r27: hex(0x15)
			add		r26,r27		;adds values of r26 and r27 and stores in r26: hex(0x2E)
			sub		r26,r27		;subtracts values of r27 from r26 and stores in r26: hex(0x19)
			mul		r26,r27		;multiplies values of r26 and r27 and stores high byte in r1 and low byte in r0: r1 = hex(0x02); r0 = hex(0x0D); decimal = 525
			neg		r26			;changes value of r26 to be negative: old = hex(0x19); new = hex(0xE7)
			ser		r27			;sets r27 to hex(0xFF)
			clr		r27			;sets r27 to hex(0x00)
			ldi		r30,0x00	;sets low byte of z-register to 0
			ldi		r31,0x01	;sets high byte of z-register to 1
			st		z,r26		;transfers value of r26 to the indirect SRAM location z, which is at address 0x100; data(0x0100) = e7
			ld		r28,z		;transfers data value at z to r28: r28 = hex(0xE7)
			mov		r29,r28		;copies value at r28 to r29: r28 = hex(0xE7); r29 = hex(0xE7)
			bset	2			;sets the negative flag
			brmi	negative	;branches to the 'negative' part of the code if the negative flag is set (it is)
			nop					;filler code that does nothing (skipped)
			nop					;filler code that does nothing (skipped)
negative:	bclr	2			;clears the negative flag
			jmp		continue	;jumps to the 'continue' part of the code unconditionally
			nop					;filler code that does nothing (skipped)
			nop					;filler code that does nothing (skipped)
continue:	bclr	0			;clears the carry bit in the status register
			bset	0			;sets the carry bit in the status register
			swap	r29			;swaps the nibbles of r29: old = hex(0xE7); new = hex(0x7E)
			ldi		r26,5		;sets value of r26 to integer 5 in preparation for the do-until loop: r26 = hex(0x05)
do:			cpi		r26,1		;compares the value of r26 to the integer 1
			brlo	until		;branches to 'until' if the value at r26 is lower than 1
			dec		r26			;decrements r26 by 1 if code did not branch to 'until'
			jmp		do			;jumps unconditionally to 'do'
until:		nop					;does nothing but executes upon completion of the do loop
			ldi		r26,0		;sets value of r26 to integer 0 in preparation for the for-next loop: r26 = hex(0x00)
			ldi		r27,5		;sets value of r27 to integer 5 in preparation for the for-next loop: r27 = hex(0x05)
for:		inc		r26			;increments the value of r26 by 1
			cp		r26,r27		;compares the value of r26 to the value of r27
			brlo	for			;branches back to 'for' if the value of r26 is less than the value of r27
next:		nop					;does nothing but executes upon completion of the for loop
			.exit				;exits the code