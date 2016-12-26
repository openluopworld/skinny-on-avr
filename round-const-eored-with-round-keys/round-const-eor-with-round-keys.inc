/*
 * round-const-eor-with-round-keys.inc
 * 
 * optimized implementation on ATmega644
 * 
 * round constants are eor-ed with round keys in key schedule,
 * then the round keys are stroed in RAM.
 * since only 8 bytes are eor-ed each round,
 * the size of round-key is 320-byte.
 *
 *  Created: 2016/12/22 9:45:58
 *   Author: luopeng
 *
 * http://www.atmel.com/webdoc/avrassembler/index.html
 */ 

.EQU	SBOX_NUM_BYTE = 256
.EQU	ENC_DEC_ROUNDS = 40
.EQU    PTEXT_NUM_BYTE = 16
.EQU    KEY_NUM_BYTE = 320

.def s0 	=	r0
.def s1 	=	r1
.def s2 	=	r2
.def s3 	=	r3
.def s4 	=	r4
.def s5 	=	r5
.def s6 	=	r6
.def s7 	=	r7
.def s8 	=	r8
.def s9 	=	r9
.def s10 	=	r10
.def s11 	=	r11
.def s12	=	r12
.def s13	=	r13
.def s14	=	r14
.def s15	=	r15

.def k0 	=	r0
.def k1 	=	r1
.def k2 	=	r2
.def k3 	=	r3
.def k4 	=	r4
.def k5 	=	r5
.def k6 	=	r6
.def k7 	=	r7
.def k8 	=	r8
.def k9 	=	r9
.def k10 	=	r10
.def k11 	=	r11
.def k12	=	r12
.def k13	=	r13
.def k14	=	r14
.def k15	=	r15

.def t0		=	r16
.def t1		=	r17
.def count  =	r18
.def two	=	r19

.def tk0	=	r20
.def tk4	=	r21

/*******************************************/
/* key schedule ****************************/
.macro load_keys
	lpm	k0,		z+
	lpm	k1,		z+
	lpm	k2,		z+
	lpm	k3,		z+
	lpm	k4,		z+
	lpm	k5,		z+
	lpm	k6,		z+
	lpm	k7,		z+
	lpm	k8,		z+
	lpm	k9,		z+
	lpm	k10,	z+
	lpm	k11,	z+
	lpm	k12,	z+
	lpm	k13,	z+
	lpm	k14,	z+
	lpm	k15,	z+
.endmacro

.macro store_round_keys
	st	x+,		tk0
	st	x+,		k1
	st	x+,		k2
	st	x+,		k3
	st	x+,		tk4
	st	x+,		k5
	st	x+,		k6
	st	x+,		k7
.endmacro

.macro add_round_const
	lpm		t0,		z+
	mov		t1,		t0
	andi	t1,		0x0f
	mov		tk0,	k0
	eor 	tk0,	t1
	andi	t0,		0x30
	swap	t0
	mov		tk4,	k4
	eor 	tk4,	t0
.endmacro

; 0  1  2  3         9  15 8  13
; 4  5  6  7         10 14 12 11
; 8  9  10 11 -----> 0  1  2  3
; 12 13 14 15        4  5  6  7
key_schedule:
	ldi		count,	ENC_DEC_ROUNDS
key_schedule_start:
	add_round_const
	store_round_keys
	dec		count
	breq	key_schedule_exit
	mov		t0,		k0
	mov		k0,		k9	
	mov		k9,		k1
	mov		k1,		k15
	mov		k15,	k7
	mov		k7,		k11
	mov		k11,	k3
	mov		k3,		k13
	mov		k13,	k5
	mov		k5,		k14
	mov		k14,	k6
	mov		k6,		k12
	mov		k12,	k4
	mov		k4,		k10
	mov		k10,	k2
	mov		k2,		k8
	mov		k8,		t0
	rjmp	key_schedule_start
key_schedule_exit:
	ret
/* key schedule end *************************/


/********************************************/
/* encrypt **********************************/
.macro load_plaintext
	lpm	s0,		z+
	lpm	s1,		z+
	lpm	s2,		z+
	lpm	s3,		z+
	lpm	s4,		z+
	lpm	s5,		z+
	lpm	s6,		z+
	lpm	s7,		z+
	lpm	s8,		z+
	lpm	s9,		z+
	lpm	s10,	z+
	lpm	s11,	z+
	lpm	s12,	z+
	lpm	s13,	z+
	lpm	s14,	z+
	lpm	s15,	z+
.endmacro

.macro store_ciphertext
	st	z+,		s0
	st	z+,		s1
	st	z+,		s2
	st	z+,		s3
	st	z+,		s7
	st	z+,		s4
	st	z+,		s5
	st	z+,		s6
	st	z+,		s10
	st	z+,		s11
	st	z+,		s8
	st	z+,		s9
	st	z+,		s13
	st	z+,		s14
	st	z+,		s15
	st	z+,		s12
.endmacro

.macro sub_column_first
	mov		r30,	s0
	ld		s0,		z
	mov		r30,	s1
	ld		s1,		z
	mov		r30,	s2
	ld		s2,		z
	mov		r30,	s3
	ld		s3,		z
	mov		r30,	s4
	ld		s4,		z
	mov		r30,	s5
	ld		s5,		z
	mov		r30,	s6
	ld		s6,		z
	mov		r30,	s7
	ld		s7,		z
	mov		r30,	s8
	ld		s8,		z
	mov		r30,	s9
	ld		s9,		z
	mov		r30,	s10
	ld		s10,	z
	mov		r30,	s11
	ld		s11,	z
	mov		r30,	s12
	ld		s12,	z
	mov		r30,	s13
	ld		s13,	z
	mov		r30,	s14
	ld		s14,	z
	mov		r30,	s15
	ld		s15,	z
.endmacro

.macro add_key
	ld		t0,		x+
	eor 	s0,		t0
	ld		t0,		x+
	eor 	s1,		t0
	ld		t0,		x+
	eor 	s2,		t0
	ld		t0,		x+
	eor 	s3,		t0
	ld		t0,		x+
	eor 	s4,		t0
	ld		t0,		x+
	eor 	s5,		t0
	ld		t0,		x+
	eor 	s6,		t0
	ld		t0,		x+
	eor 	s7,		t0
	eor		s8,		two
.endmacro

; 0  1  2  3         0  1  2  3
; 4  5  6  7         7  4  5  6
; 8  9  10 11 -----> 10 11 8  9
; 12 13 14 15        13 14 15 12
; eor 	s4,		s8
; eor 	s8,		s0
; eor 	s12,	s8
; mov	t0,		s12
; mov	s12,	s8
; mov	s8,		s4
; mov	s4,		s0
; mov	s0,		t0
.macro shift_row_mix_column
	; first column
	eor 	s7,		s10
	eor 	s10,	s0
	eor 	s13,	s10
	mov		t0,		s13
	mov		s13,	s10
	mov		s10,	s7
	mov		s7,		s0
	mov		s0,		t0
	; second column
	eor 	s4,		s11
	eor 	s11,	s1
	eor 	s14,	s11
	mov		t0,		s14
	mov		s14,	s11
	mov		s11,	s4
	mov		s4,		s1
	mov		s1,		t0
	; third column
	eor 	s5,		s8
	eor 	s8,		s2
	eor 	s15,	s8
	mov		t0,		s15
	mov		s15,	s8
	mov		s8,		s5
	mov		s5,		s2
	mov		s2,		t0
	; fourth column 3 6 9 12
	eor 	s6,		s9
	eor 	s9,		s3
	eor 	s12,	s9
	mov		t0,		s12
	mov		s12,	s9
	mov		s9,		s6
	mov		s6,		s3
	mov		s3,		t0
.endmacro

;  0  1  2  3         0  1  2  3  
;  7  4  5  6         4  5  6  7 
;  10 11 8  9  -----> 8  9  10 11
;  13 14 15 12        12 13 14 15 
.macro sub_column_with_shift_row
	; row 1
	mov		r30,	s0
	ld		s0,		z
	mov		r30,	s1
	ld		s1,		z
	mov		r30,	s2
	ld		s2,		z
	mov		r30,	s3
	ld		s3,		z
	; row 2
	mov		r30,	s4
	ld		t0,		z
	mov		r30,	s7
	ld		s4,		z
	mov		r30,	s6
	ld		s7,		z
	mov		r30,	s5
	ld		s6,		z
	mov		s5,		t0
	; row 3
	mov		r30,	s8
	ld		t0,		z
	mov		r30,	s10
	ld		s8,		z
	mov		s10,	t0
	mov		r30,	s9
	ld		t0,		z
	mov		r30,	s11
	ld		s9,		z
	mov		s11,	t0
	; row 4
	mov		r30,	s12
	ld		t0,		z
	mov		r30,	s13
	ld		s12,	z
	mov		r30,	s14
	ld		s13,	z
	mov		r30,	s15
	ld		s14,	z
	mov		s15,	t0
.endmacro

encrypt:
	ldi	two,	0x02
	ldi count,	ENC_DEC_ROUNDS
	sub_column_first
enc_round:
	add_key
	shift_row_mix_column
	dec	count
	breq exit
	sub_column_with_shift_row
	rjmp enc_round
exit:
	ret