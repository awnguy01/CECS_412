/*
* L2P3_C.c
* Example relationship between C and Assembly
* Created: 1/26/2018 4:56:28 PM * Author : Eugene Rockey */

//Compile and examine the .lss file beginning with main. Comment all the lines the compiler left empty.
int Global_A;
int Global_B = 1;
int Global_C = 2;
void main(void)
{
Global_A = Global_C / Global_B; 
}

/*
 a0:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <Global_C>
 a4:	90 91 01 01 	lds	r25, 0x0101	; 0x800101 <Global_C+0x1>
 a8:	60 91 02 01 	lds	r22, 0x0102	; 0x800102 <Global_B>
 ac:	70 91 03 01 	lds	r23, 0x0103	; 0x800103 <Global_B+0x1>
 b0:	05 d0       	rcall	.+10     	; 0xbc <__divmodhi4>
 b2:	70 93 05 01 	sts	0x0105, r23	; 0x800105 <__data_end+0x1>
 b6:	60 93 04 01 	sts	0x0104, r22	; 0x800104 <__data_end>
 ba:	08 95       	ret
*/

unsigned int Global_A;
unsigned int Global_B = 1;
unsigned int Global_C = 2;
void main(void)
{
	Global_A = Global_C / Global_B;
}

/*

  a0:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <Global_C>
  a4:	90 91 01 01 	lds	r25, 0x0101	; 0x800101 <Global_C+0x1>
  a8:	60 91 02 01 	lds	r22, 0x0102	; 0x800102 <Global_B>
  ac:	70 91 03 01 	lds	r23, 0x0103	; 0x800103 <Global_B+0x1>
  b0:	05 d0       	rcall	.+10     	; 0xbc <__udivmodhi4>
  b2:	70 93 05 01 	sts	0x0105, r23	; 0x800105 <__data_end+0x1>
  b6:	60 93 04 01 	sts	0x0104, r22	; 0x800104 <__data_end>
  ba:	08 95       	ret
*/

//Compile and examine the .lss file beginning with main, comment all the lines the compiler left empty.
char Global_A;
char Global_B = 1;
char Global_C = 2;
void main(void)
{
	Global_A = Global_C / Global_B; 
}
*/
/*
 a0:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <Global_C>
 a4:	08 2e       	mov	r0, r24		; the value of Global_C is copied to r0
 a6:	00 0c       	add	r0, r0		; r0 is doubled to test for a carry bit
 a8:	99 0b       	sbc	r25, r25	; r25 is subtracted from itself minus the carry bit if applicable to set the low byte
 aa:	60 91 01 01 	lds	r22, 0x0101	; 0x800101 <Global_B>
 ae:	06 2e       	mov	r0, r22		; the value of Global _B is copied to r0
 b0:	00 0c       	add	r0, r0		; ro is doubled to test for a carry bit
 b2:	77 0b       	sbc	r23, r23	; r23 is subtracted from itself minus the carry bit if applicable to set the low byte
 b4:	03 d0       	rcall	.+6      	; 0xbc <__divmodhi4>
 b6:	60 93 02 01 	sts	0x0102, r22	; 0x800102 <__data_end>
 ba:	08 95       	ret
*/

unsigned char Global_A;
unsigned char Global_B = 1;
unsigned char Global_C = 2;
void main(void)
{
	Global_A = Global_C / Global_B;
}
/*
  a0:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <Global_C>
  a4:	60 91 01 01 	lds	r22, 0x0101	; 0x800101 <Global_B>
  a8:	03 d0       	rcall	.+6      	; 0xb0 <__udivmodqi4>
  aa:	80 93 02 01 	sts	0x0102, r24	; 0x800102 <__data_end>
  ae:	08 95       	ret
*/