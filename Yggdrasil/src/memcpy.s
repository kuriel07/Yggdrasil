;/*
; * ARM memcpy asm replacement.
; *
; * Copyright (C) 2009 Bluush Dev Team.
; *
; * This library is free software; you can redistribute it and/or
; * modify it under the terms of the GNU Lesser General Public
; * License as published by the Free Software Foundation; either
; * version 2 of the License, or (at your option) any later version.
; *
; * This library is distributed in the hope that it will be useful,
; * but WITHOUT ANY WARRANTY; without even the implied warranty of
; * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
; * Lesser General Public License for more details.
; *
; * You should have received a copy of the GNU Lesser General Public
; * License along with this library; if not, write to the
; * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
; * Boston, MA 02111-1307, USA.
; */

;#if USE_ARMASM && !WORDS_BIGENDIAN

;#define _LABEL(f)   f :

	EXPORT m_memcopy
	EXPORT m_memcopy_mask
	AREA |C$$code|, CODE, READONLY
	CODE32

m_memcopy	
	cmp r1, r0
	bcc Lmemcpy_backwards

	moveq r0, #0
	moveq pc, lr
	
	stmdb sp!, {r0, lr}
	subs r2, r2, #4
	blt Lmemcpy_fl4
	ands r12, r0, #3
	bne Lmemcpy_fdestul
	ands r12, r1, #3
	bne Lmemcpy_fsrcul

Lmemcpy_ft8
	subs r2, r2, #8
	blt Lmemcpy_fl12
	subs r2, r2, #0x14        
	blt Lmemcpy_fl32
	stmdb sp!, {r4}

Lmemcpy_floop32
	ldmia r1!, {r3, r4, r12, lr}
	stmia r0!, {r3, r4, r12, lr}
	ldmia r1!, {r3, r4, r12, lr}
	stmia r0!, {r3, r4, r12, lr}
	subs r2, r2, #0x20        
	bge Lmemcpy_floop32

	cmn r2, #0x10
	ldmgeia r1!, {r3, r4, r12, lr}
	stmgeia r0!, {r3, r4, r12, lr}
	subge r2, r2, #0x10        
	ldmia sp!, {r4}

Lmemcpy_fl32
	adds r2, r2, #0x14        


Lmemcpy_floop12
	ldmgeia r1!, {r3, r12, lr}
	stmgeia r0!, {r3, r12, lr}
	subges r2, r2, #0x0c        
	bge Lmemcpy_floop12

Lmemcpy_fl12
	adds r2, r2, #8
	blt Lmemcpy_fl4

	subs r2, r2, #4
	ldrlt r3, [r1], #4
	strlt r3, [r0], #4
	ldmgeia r1!, {r3, r12}
	stmgeia r0!, {r3, r12}
	subge r2, r2, #4

Lmemcpy_fl4
	adds r2, r2, #4
	ldmeqia sp!, {r0, pc}
	
	cmp r2, #2
	ldrb r3, [r1], #1
	strb r3, [r0], #1
	ldrgeb r3, [r1], #1
	strgeb r3, [r0], #1
	ldrgtb r3, [r1], #1
	strgtb r3, [r0], #1
	ldmia sp!, {r0, pc}


Lmemcpy_fdestul
	rsb r12, r12, #4
	cmp r12, #2
	
	ldrb r3, [r1], #1
	strb r3, [r0], #1
	ldrgeb r3, [r1], #1
	strgeb r3, [r0], #1
	ldrgtb r3, [r1], #1
	strgtb r3, [r0], #1
	subs r2, r2, r12
	blt Lmemcpy_fl4
	
	ands r12, r1, #3
	beq Lmemcpy_ft8


Lmemcpy_fsrcul
	bic r1, r1, #3
	ldr lr, [r1], #4
	cmp r12, #2
	bgt Lmemcpy_fsrcul3
	beq Lmemcpy_fsrcul2
	cmp r2, #0x0c            
	blt Lmemcpy_fsrcul1loop4
	sub r2, r2, #0x0c        
	stmdb sp!, {r4, r5}

Lmemcpy_fsrcul1loop16
	mov r3, lr, lsr #8
	ldmia r1!, {r4, r5, r12, lr}
	orr r3, r3, r4, lsl #24
	mov r4, r4, lsr #8
	orr r4, r4, r5, lsl #24
	mov r5, r5, lsr #8
	orr r5, r5, r12, lsl #24
	mov r12, r12, lsr #8
	orr r12, r12, lr, lsl #24
	stmia r0!, {r3-r5, r12}
	subs r2, r2, #0x10        
	bge Lmemcpy_fsrcul1loop16
	ldmia sp!, {r4, r5}
	adds r2, r2, #0x0c        
	blt Lmemcpy_fsrcul1l4

Lmemcpy_fsrcul1loop4
	mov r12, lr, lsr #8
	ldr lr, [r1], #4
	orr r12, r12, lr, lsl #24
	str r12, [r0], #4
	subs r2, r2, #4
	bge Lmemcpy_fsrcul1loop4

Lmemcpy_fsrcul1l4
	sub r1, r1, #3
	b Lmemcpy_fl4

Lmemcpy_fsrcul2
	cmp r2, #0x0c            
	blt Lmemcpy_fsrcul2loop4
	sub r2, r2, #0x0c        
	stmdb sp!, {r4, r5}

Lmemcpy_fsrcul2loop16
	mov r3, lr, lsr #16
	ldmia r1!, {r4, r5, r12, lr}
	orr r3, r3, r4, lsl #16
	mov r4, r4, lsr #16
	orr r4, r4, r5, lsl #16
	mov r5, r5, lsr #16
	orr r5, r5, r12, lsl #16
	mov r12, r12, lsr #16
	orr r12, r12, lr, lsl #16
	stmia r0!, {r3-r5, r12}
	subs r2, r2, #0x10        
	bge Lmemcpy_fsrcul2loop16
	ldmia sp!, {r4, r5}
	adds r2, r2, #0x0c        
	blt Lmemcpy_fsrcul2l4

Lmemcpy_fsrcul2loop4
	mov r12, lr, lsr #16
	ldr lr, [r1], #4
	orr r12, r12, lr, lsl #16
	str r12, [r0], #4
	subs r2, r2, #4
	bge Lmemcpy_fsrcul2loop4

Lmemcpy_fsrcul2l4
	sub r1, r1, #2
	b Lmemcpy_fl4

Lmemcpy_fsrcul3
	cmp r2, #0x0c            
	blt Lmemcpy_fsrcul3loop4
	sub r2, r2, #0x0c        
	stmdb sp!, {r4, r5}

Lmemcpy_fsrcul3loop16
	mov r3, lr, lsr #24
	ldmia r1!, {r4, r5, r12, lr}
	orr r3, r3, r4, lsl #8
	mov r4, r4, lsr #24
	orr r4, r4, r5, lsl #8
	mov r5, r5, lsr #24
	orr r5, r5, r12, lsl #8
	mov r12, r12, lsr #24
	orr r12, r12, lr, lsl #8
	stmia r0!, {r3-r5, r12}
	subs r2, r2, #0x10        
	bge Lmemcpy_fsrcul3loop16
	ldmia sp!, {r4, r5}
	adds r2, r2, #0x0c        
	blt Lmemcpy_fsrcul3l4

Lmemcpy_fsrcul3loop4
	mov r12, lr, lsr #24
	ldr lr, [r1], #4
	orr r12, r12, lr, lsl #8
	str r12, [r0], #4
	subs r2, r2, #4
	bge Lmemcpy_fsrcul3loop4

Lmemcpy_fsrcul3l4
	sub r1, r1, #1
	b Lmemcpy_fl4

Lmemcpy_backwards
	add r1, r1, r2
	add r0, r0, r2
	subs r2, r2, #4
	blt Lmemcpy_bl4
	ands r12, r0, #3
	bne Lmemcpy_bdestul
	ands r12, r1, #3
	bne Lmemcpy_bsrcul

Lmemcpy_bt8
	subs r2, r2, #8
	blt Lmemcpy_bl12
	stmdb sp!, {r4, lr}
	subs r2, r2, #0x14
	blt Lmemcpy_bl32


Lmemcpy_bloop32
	ldmdb r1!, {r3, r4, r12, lr}
	stmdb r0!, {r3, r4, r12, lr}
	ldmdb r1!, {r3, r4, r12, lr}
	stmdb r0!, {r3, r4, r12, lr}
	subs r2, r2, #0x20        
	bge Lmemcpy_bloop32

Lmemcpy_bl32
	cmn r2, #0x10            
	ldmgedb r1!, {r3, r4, r12, lr}
	stmgedb r0!, {r3, r4, r12, lr}
	subge r2, r2, #0x10        
	adds r2, r2, #0x14        
	ldmgedb r1!, {r3, r12, lr}
	stmgedb r0!, {r3, r12, lr}
	subge r2, r2, #0x0c        
	ldmia sp!, {r4, lr}

Lmemcpy_bl12
	adds r2, r2, #8
	blt Lmemcpy_bl4
	subs r2, r2, #4
	ldrlt r3, [r1, #-4]!
	strlt r3, [r0, #-4]!
	ldmgedb r1!, {r3, r12}
	stmgedb r0!, {r3, r12}
	subge r2, r2, #4

Lmemcpy_bl4
	adds r2, r2, #4
	moveq pc, lr

	cmp r2, #2
	ldrb r3, [r1, #-1]!
	strb r3, [r0, #-1]!
	ldrgeb r3, [r1, #-1]!
	strgeb r3, [r0, #-1]!
	ldrgtb r3, [r1, #-1]!
	strgtb r3, [r0, #-1]!
	mov pc, lr


Lmemcpy_bdestul
	cmp r12, #2

	ldrb r3, [r1, #-1]!
	strb r3, [r0, #-1]!
	ldrgeb r3, [r1, #-1]!
	strgeb r3, [r0, #-1]!
	ldrgtb r3, [r1, #-1]!
	strgtb r3, [r0, #-1]!
	subs r2, r2, r12
	blt Lmemcpy_bl4
	ands r12, r1, #3
	beq Lmemcpy_bt8


Lmemcpy_bsrcul
	bic r1, r1, #3
	ldr r3, [r1, #0]
	cmp r12, #2
	blt Lmemcpy_bsrcul1
	beq Lmemcpy_bsrcul2
	cmp r2, #0x0c            
	blt Lmemcpy_bsrcul3loop4
	sub r2, r2, #0x0c        
	stmdb sp!, {r4, r5, lr}

Lmemcpy_bsrcul3loop16
	mov lr, r3, lsl #8
	ldmdb r1!, {r3-r5, r12}
	orr lr, lr, r12, lsr #24
	mov r12, r12, lsl #8
	orr r12, r12, r5, lsr #24
	mov r5, r5, lsl #8
	orr r5, r5, r4, lsr #24
	mov r4, r4, lsl #8
	orr r4, r4, r3, lsr #24
	stmdb r0!, {r4, r5, r12, lr}
	subs r2, r2, #0x10        
	bge Lmemcpy_bsrcul3loop16
	ldmia sp!, {r4, r5, lr}
	adds r2, r2, #0x0c        
	blt Lmemcpy_bsrcul3l4

Lmemcpy_bsrcul3loop4
	mov r12, r3, lsl #8
	ldr r3, [r1, #-4]!
	orr r12, r12, r3, lsr #24
	str r12, [r0, #-4]!
	subs r2, r2, #4
	bge Lmemcpy_bsrcul3loop4

Lmemcpy_bsrcul3l4
	add r1, r1, #3
	b Lmemcpy_bl4

Lmemcpy_bsrcul2
	cmp r2, #0x0c            
	blt Lmemcpy_bsrcul2loop4
	sub r2, r2, #0x0c        
	stmdb sp!, {r4, r5, lr}

Lmemcpy_bsrcul2loop16
	mov lr, r3, lsl #16
	ldmdb r1!, {r3-r5, r12}
	orr lr, lr, r12, lsr #16
	mov r12, r12, lsl #16
	orr r12, r12, r5, lsr #16
	mov r5, r5, lsl #16
	orr r5, r5, r4, lsr #16
	mov r4, r4, lsl #16
	orr r4, r4, r3, lsr #16
	stmdb r0!, {r4, r5, r12, lr}
	subs r2, r2, #0x10        
	bge Lmemcpy_bsrcul2loop16
	ldmia sp!, {r4, r5, lr}
	adds r2, r2, #0x0c        
	blt Lmemcpy_bsrcul2l4

Lmemcpy_bsrcul2loop4
	mov r12, r3, lsl #16
	ldr r3, [r1, #-4]!
	orr r12, r12, r3, lsr #16
	str r12, [r0, #-4]!
	subs r2, r2, #4
	bge Lmemcpy_bsrcul2loop4

Lmemcpy_bsrcul2l4
	add r1, r1, #2
	b Lmemcpy_bl4

Lmemcpy_bsrcul1
	cmp r2, #0x0c            
	blt Lmemcpy_bsrcul1loop4
	sub r2, r2, #0x0c        
	stmdb sp!, {r4, r5, lr}

Lmemcpy_bsrcul1loop32
	mov lr, r3, lsl #24
	ldmdb r1!, {r3-r5, r12}
	orr lr, lr, r12, lsr #8
	mov r12, r12, lsl #24
	orr r12, r12, r5, lsr #8
	mov r5, r5, lsl #24
	orr r5, r5, r4, lsr #8
	mov r4, r4, lsl #24
	orr r4, r4, r3, lsr #8
	stmdb r0!, {r4, r5, r12, lr}
	subs r2, r2, #0x10        
	bge Lmemcpy_bsrcul1loop32
	ldmia sp!, {r4, r5, lr}
	adds r2, r2, #0x0c        
	blt Lmemcpy_bsrcul1l4

Lmemcpy_bsrcul1loop4
	mov r12, r3, lsl #24
	ldr r3, [r1, #-4]!
	orr r12, r12, r3, lsr #8
	str r12, [r0, #-4]!
	subs r2, r2, #4
	bge Lmemcpy_bsrcul1loop4

Lmemcpy_bsrcul1l4
	add r1, r1, #1
	b Lmemcpy_bl4
	
cast2uint16	dcd	0xffff
m_memcopy_mask
	;push registers
	stmfd 	sp!,{r0-r7,lr}
m_memcopy_mask_loop
	stmfd 	sp!,{r4-r5}
	add		r4, r4, r0, lsl #1
	add 	r5, r1, r5
	ldr 	r7, [r5]
	ldr 	r6, cast2uint16
	ldr		r6,	[r6]
	and		r7, r7, r6, lsl #0
	cmp		r7, r3
	strne	r7, [r4]	
	ldmfd 	sp, {r4-r5}
	cmp 	r0, r2
	beq	m_memcopy_mask_ret
	add		r0, r0, #1
	add		r1, r0, #2
	b	m_memcopy_mask_loop
	;ret
m_memcopy_mask_ret
	ldmfd	sp, {r0-r7, pc}
	
;	.ltorg
	END	
;#endif /* USE_ARMASM && !WORDS_BIGENDIAN */