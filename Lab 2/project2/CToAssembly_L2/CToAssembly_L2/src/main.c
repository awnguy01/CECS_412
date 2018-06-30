unsigned char Global_A; 
unsigned char Global_B = 1; 
unsigned char Global_C = 2;

void main(void) { 
	Global_A = Global_C % Global_B; 
} 

/*
;000000a0 <main>:
					lds	r24, 0x0100			; 0x800100 <Global_C>
					lds	r22, 0x0101			; 0x800101 <Global_B>
					rcall	 .+6      		; 0xb0 <__udivmodqi4>
					sts		 0x0102, r25	; 0x800102 <__data_end>
ae:	08 95			ret						; return from the main function
;000000b0 <__udivmodqi4>:
b0:	99 1b       	sub	r25, r25			; clears r25 of any values
b2:	79 e0       	ldi	r23, 0x09			; 9
b4:	04 c0       	rjmp	.+8      		; 0xbe <__udivmodqi4_ep>
;000000b6 <__udivmodqi4_loop>:
b6:	99 1f       	adc	r25, r25			; doubles value in r25 and adds the carry bit if set
b8:	96 17       	cp	r25, r22			; compares value in r25 with value in r22
ba:	08 f0       	brcs	.+2      		; 0xbe <__udivmodqi4_ep>
bc:	96 1b       	sub	r25, r22			; if value in r25 greater than value in r22 then subtract r22 from r25 and store in r25
;000000be <__udivmodqi4_ep>:
be:	88 1f       	adc	r24, r24			; doubles value in r24 and adds the carry bit if set
c0:	7a 95       	dec	r23					; decrements the value in r23 by 1
c2:	c9 f7       	brne	.-14     		; 0xb6 <__udivmodqi4_loop>
c4:	80 95       	com	r24					; takes the ones complement of r24
c6:	08 95       	ret						; returns from the subroutine
;000000c8 <_exit>:
c8:	f8 94       	cli						; clear global interrupt flag
;000000ca <__stop_program>:
ca:	ff cf       	rjmp	.-2      		; 0xca <__stop_program>
*/
