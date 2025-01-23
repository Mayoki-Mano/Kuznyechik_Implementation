section .data
extern LS_mat               ; Объявляем глобальный массив LS_mat

section .text
global LS_asm                   ; Объявляем функцию LS как глобальную



; Arguments:
;   rcx - in_out (uint8_t*), a pointer to the input/output data
LS_asm:
    mov rdx, LS_mat         ; Load the base address of LS_mat into rdx
    movzx r10, byte[rcx]    ; in_out[0] -> r10
    shl r10, 4              ; in_out[0] * 16 -> r10
    movaps xmm0, [rdx+r10]  ; result = LS_mat[0][in_out[0]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[1] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[1] -> r10
    shl r10, 4              ; in_out[1] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[1][in_out[1]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[2] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[2] -> r10
    shl r10, 4              ; in_out[2] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[2][in_out[2]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[3] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[3] -> r10
    shl r10, 4              ; in_out[3] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[3][in_out[3]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[4] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[4] -> r10
    shl r10, 4              ; in_out[4] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[4][in_out[4]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[5] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[5] -> r10
    shl r10, 4              ; in_out[5] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[5][in_out[5]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[6] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[6] -> r10
    shl r10, 4              ; in_out[6] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[6][in_out[6]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[7] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[7] -> r10
    shl r10, 4              ; in_out[7] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[7][in_out[7]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[8] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[8] -> r10
    shl r10, 4              ; in_out[8] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[8][in_out[8]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[9] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[9] -> r10
    shl r10, 4              ; in_out[9] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[9][in_out[9]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[10] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[10] -> r10
    shl r10, 4              ; in_out[10] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[10][in_out[10]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[11] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[11] -> r10
    shl r10, 4              ; in_out[11] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[11][in_out[11]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[12] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[12] -> r10
    shl r10, 4              ; in_out[12] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[12][in_out[12]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[13] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[13] -> r10
    shl r10, 4              ; in_out[13] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[13][in_out[13]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[14] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[14] -> r10
    shl r10, 4              ; in_out[14] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[14][in_out[14]] (chunk of 16 bytes)
    add rdx, 4096           ; LS_mat[15] -> rcx
    inc rcx                 ; Move to the next in_out element
    movzx r10, byte[rcx]    ; in_out[15] -> r10
    shl r10, 4              ; in_out[15] * 16 -> r10
    pxor xmm0, [rdx+r10]    ; result ^= LS_mat[15][in_out[15]] (chunk of 16 bytes)
    sub rcx, 15             ; Restore the initial value of rcx
    movaps [rcx], xmm0      ; Store the result back to in_out
    ret