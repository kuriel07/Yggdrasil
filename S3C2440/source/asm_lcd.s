	export Buffer_Copy_Key1
	export f_fast_mem_blit	
	area |C$$code|, code, readonly
	code32

;r0 = key
;r1 = dst
;r2 = src
;r3 = size
;memory must be aligned 2 byte

f_fast_mem_blit	
	cmp r2, r1
	moveq r0, #0			;jika r2 equal r1
	moveq pc, lr			;jika r2 equal r1 [ret]
	bcc Lfast_blit_backwards	;jika r2 < r1
	
	stmdb sp!, {r1-r4, lr}		;push r1-r4, lr
	subs r3, r3, #8				;size -= 8 + compare
	blt Lfast_blit_fl8			;jika size < 4 goto Lmemcpy_fl4

Lfast_blit_floop8
	;1
	ldrh r4, [r2, #1]!		;copy 2 byte src	
	cmp  r4, r0				;compare r4 dengan alpha key
	add  r1, r1, #1			;tambah r1 += 2
	strneh r4, [r1]			;paste 2 byte dst if not equal
	;2
	ldrh r4, [r2, #1]!		;copy 2 byte src	
	cmp  r4, r0				;compare r4 dengan alpha key
	add  r1, r1, #1			;tambah r1 += 2
	strneh r4, [r1]			;paste 2 byte dst if not equal
	;3
	ldrh r4, [r2, #1]!		;copy 2 byte src	
	cmp  r4, r0				;compare r4 dengan alpha key
	add  r1, r1, #1			;tambah r1 += 2
	strneh r4, [r1]			;paste 2 byte dst if not equal
	;4
	ldrh r4, [r2, #1]!		;copy 2 byte src	
	cmp  r4, r0				;compare r4 dengan alpha key
	add  r1, r1, #1			;tambah r1 += 2
	strneh r4, [r1]			;paste 2 byte dst if not equal
	;5
	ldrh r4, [r2, #1]!		;copy 2 byte src	
	cmp  r4, r0				;compare r4 dengan alpha key
	add  r1, r1, #1			;tambah r1 += 2
	strneh r4, [r1]			;paste 2 byte dst if not equal
	;6
	ldrh r4, [r2, #1]!		;copy 2 byte src	
	cmp  r4, r0				;compare r4 dengan alpha key
	add  r1, r1, #1			;tambah r1 += 2
	strneh r4, [r1]			;paste 2 byte dst if not equal
	;7
	ldrh r4, [r2, #1]!		;copy 2 byte src	
	cmp  r4, r0				;compare r4 dengan alpha key
	add  r1, r1, #1			;tambah r1 += 2
	strneh r4, [r1]			;paste 2 byte dst if not equal
	;8
	ldrh r4, [r2, #1]!		;copy 2 byte src	
	cmp  r4, r0				;compare r4 dengan alpha key
	add  r1, r1, #1			;tambah r1 += 2
	strneh r4, [r1]			;paste 2 byte dst if not equal

	subs r3, r3, #8        		;size -= 8
	bge Lfast_blit_floop8		;jika size >= 8 loop Lfast_blit_floop8

Lfast_blit_fl8
	adds r3, r3, #8				;add r3 += 4 and compare
	ldmeqia sp!, {r1-r4, pc}	;return jika r3 == 0
	
Lfast_blit_fl8_loop	
	ldrh r4, [r2, #1]!		;copy 2 byte src	
	cmp  r4, r0				;compare r4 dengan alpha key
	add  r1, r1, #1			;tambah r1 += 2
	strneh r4, [r1]			;paste 2 byte dst if not equal
	subs r3, r3, #1
	bne  Lfast_blit_fl8_loop	;jika r3 tidak sama dengan 0 loop
	ldmia sp!, {r1-r4, pc}		;return;

Lfast_blit_backwards
	add r2, r2, r3		;src = src + size
	add r1, r1, r3		;dst = dst + size
	subs r3, r3, #8		;size -=8 + compare		
	blt Lfast_blit_bl8

Lfast_blit_bloop8
	;1
	ldrh r4, [r2, #-1]!		;copy 2 byte src, src -= 2
	cmp  r4, r0				;compare r4 dengan alpha key
	sub  r1, r1, #1			;dst -= 2
	strneh r4, [r1]			;paste 2 byte dst
	;2
	ldrh r4, [r2, #-1]!		;copy 2 byte src, src -= 2
	cmp  r4, r0				;compare r4 dengan alpha key
	sub  r1, r1, #1			;dst -= 2
	strneh r4, [r1]			;paste 2 byte dst
	;3
	ldrh r4, [r2, #-1]!		;copy 2 byte src, src -= 2
	cmp  r4, r0				;compare r4 dengan alpha key
	sub  r1, r1, #1			;dst -= 2
	strneh r4, [r1]			;paste 2 byte dst
	;4
	ldrh r4, [r2, #-1]!		;copy 2 byte src, src -= 2
	cmp  r4, r0				;compare r4 dengan alpha key
	sub  r1, r1, #1			;dst -= 2
	strneh r4, [r1]			;paste 2 byte dst
	;5
	ldrh r4, [r2, #-1]!		;copy 2 byte src, src -= 2
	cmp  r4, r0				;compare r4 dengan alpha key
	sub  r1, r1, #1			;dst -= 2
	strneh r4, [r1]			;paste 2 byte dst
	;6
	ldrh r4, [r2, #-1]!		;copy 2 byte src, src -= 2
	cmp  r4, r0				;compare r4 dengan alpha key
	sub  r1, r1, #1			;dst -= 2
	strneh r4, [r1]			;paste 2 byte dst
	;7
	ldrh r4, [r2, #-1]!		;copy 2 byte src, src -= 2
	cmp  r4, r0				;compare r4 dengan alpha key
	sub  r1, r1, #1			;dst -= 2
	strneh r4, [r1]			;paste 2 byte dst
	;8
	ldrh r4, [r2, #-1]!		;copy 2 byte src, src -= 2
	cmp  r4, r0				;compare r4 dengan alpha key
	sub  r1, r1, #1			;dst -= 2
	strneh r4, [r1]			;paste 2 byte dst
	
	subs r3, r3, #8        		;size -= 8
	bge Lfast_blit_bloop8		;jika size >= 8 loop Lfast_blit_floop8

Lfast_blit_bl8
	adds r3, r3, #8				;add r3 += 8 and compare
	ldmeqia sp!, {r1-r4, pc}	;return jika r3 == 0
	
Lfast_blit_bl8_loop	
	ldrh r4, [r2, #-1]!		;copy 2 byte src, src -= 2
	cmp  r4, r0				;compare r4 dengan alpha key
	sub  r1, r1, #1			;dst -= 2
	strneh r4, [r1]			;paste 2 byte dst
	subs r3, r3, #1
	bne  Lfast_blit_bl8_loop		;jika r3 tidak sama dengan 0 loop
	ldmia sp!, {r1-r4, pc}			;return;

Buffer_Copy_Key1
        STMFD    r13!,{r4-r6,r14}
        LDR      r12,[r13,#0x10]
        LDR      r14,[r13,#0x14]
        MOV      r4,r2
        CMP      r14,#8
        BLS      _buffer_copy_key_remaining
_buffer_copy_key_8byte
    ;1
		ADD      r2,r4,#1
        ADD      r4,r12,r4,LSL #1
        LDRH     r4,[r4]
        BIC      r2,r2,#0x10000
        TEQ      r4,r0
        ADDNE      r5,r1,#1
        MOVNE      r6,r1
        MOVNE      r1,r5
        ADDNE      r5,r3,r6,LSL #1
        STRNEH     r4,[r5]
	;2
        ADD      r4,r2,#1
		ADD      r2,r4,#1
        ADD      r4,r12,r4,LSL #1
        LDRH     r4,[r4]
        BIC      r2,r2,#0x10000
        TEQ      r4,r0
        ADDNE      r5,r1,#1
        MOVNE      r6,r1
        MOVNE      r1,r5
        ADDNE      r5,r3,r6,LSL #1
        STRNEH     r4,[r5]
	;3
		ADD      r2,r4,#1
        ADD      r4,r12,r4,LSL #1
        LDRH     r4,[r4]
        BIC      r2,r2,#0x10000
        TEQ      r4,r0
        ADDNE      r5,r1,#1
        MOVNE      r6,r1
        MOVNE      r1,r5
        ADDNE      r5,r3,r6,LSL #1
        STRNEH     r4,[r5]
	;4
        ADD      r4,r2,#1
		ADD      r2,r4,#1
        ADD      r4,r12,r4,LSL #1
        LDRH     r4,[r4]
        BIC      r2,r2,#0x10000
        TEQ      r4,r0
        ADDNE      r5,r1,#1
        MOVNE      r6,r1
        MOVNE      r1,r5
        ADDNE      r5,r3,r6,LSL #1
        STRNEH     r4,[r5]
	;5
		ADD      r2,r4,#1
        ADD      r4,r12,r4,LSL #1
        LDRH     r4,[r4]
        BIC      r2,r2,#0x10000
        TEQ      r4,r0
        ADDNE      r5,r1,#1
        MOVNE      r6,r1
        MOVNE      r1,r5
        ADDNE      r5,r3,r6,LSL #1
        STRNEH     r4,[r5]
	;6
        ADD      r4,r2,#1
		ADD      r2,r4,#1
        ADD      r4,r12,r4,LSL #1
        LDRH     r4,[r4]
        BIC      r2,r2,#0x10000
        TEQ      r4,r0
        ADDNE      r5,r1,#1
        MOVNE      r6,r1
        MOVNE      r1,r5
        ADDNE      r5,r3,r6,LSL #1
        STRNEH     r4,[r5]
	;7
		ADD      r2,r4,#1
        ADD      r4,r12,r4,LSL #1
        LDRH     r4,[r4]
        BIC      r2,r2,#0x10000
        TEQ      r4,r0
        ADDNE      r5,r1,#1
        MOVNE      r6,r1
        MOVNE      r1,r5
        ADDNE      r5,r3,r6,LSL #1
        STRNEH     r4,[r5]
	;8
        ADD      r4,r2,#1
		ADD      r2,r4,#1
        ADD      r4,r12,r4,LSL #1
        LDRH     r4,[r4]
        BIC      r2,r2,#0x10000
        TEQ      r4,r0
        ADDNE      r5,r1,#1
        MOVNE      r6,r1
        MOVNE      r1,r5
        ADDNE      r5,r3,r6,LSL #1
        STRNEH     r4,[r5]
		SUB      	r14,r14,#8
        CMP      r14,#8
        BHI      _buffer_copy_key_8byte
_buffer_copy_key_remaining
        CMP      r14,#0
        BEQ      _buffer_copy_key_finish
_buffer_copy_key_rem_loop
        ADD      r2,r4,#1
        BIC      r2,r2,#0x10000
        MOV      r5,r4
        MOV      r4,r2
        ADD      r2,r12,r5,LSL #1
        LDRH     r2,[r2]
        TEQ      r2,r0
        ADDNE      r5,r1,#1
        MOVNE      r6,r1
        MOVNE      r1,r5
        ADDNE      r5,r3,r6,LSL #1
        STRNEH     r2,[r5]
        SUB      r14,r14,#1
        CMP      r14,#0
        BNE      _buffer_copy_key_rem_loop
_buffer_copy_key_finish
        LDMFD    r13!,{r4-r6,pc}
        
        END