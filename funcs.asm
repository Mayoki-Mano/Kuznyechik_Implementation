section .data
mask db 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF

section .bss

section .text
global LS_asm                   ; Объявляем функцию LS как глобальную
extern LS_mat               ; Объявляем глобальный массив LS_mat




; Arguments:
;   rcx - in_out (uint8_t*), a pointer to the input/output data
LS_asm:
    mov r11, LS_mat         ; Load the base address of LS_mat into r11
    movaps xmm0, [rcx]      ; xmm0 <- in_out
    lea r8, [rel mask]        ; Используем REL для корректной загрузки адреса
    movaps xmm3, [r8]       ; xmm3 <- mask
    movaps xmm2, [r8]       ; xmm2 <- mask
    pandn xmm2, xmm0         ; xmm2 <- in_out & mask - чётные байты
    pand xmm3, xmm0        ; xmm3 <- in_out & ~mask - нечётные байты
    psllw xmm2, 4           ; xmm2 <- умножение чётных байтов на 16
    psrlw xmm3, 4           ; xmm3 <- умножение нечётных байтов на 16
    pextrw eax, xmm2, 0      ; eax <- in_out[0] * 16
    movaps xmm1, [r11 + rax]; xmm1 <- LS_mat[0][in_out[0]]
    add r11, 4096           ; r11 <- LS_mat[1]
    pextrw  eax, xmm3, 0      ; eax <- in_out[1] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- LS_mat[0][in_out[0]] ^ LS_mat[1][in_out[1]]
    add r11, 4096           ; r11 <- LS_mat[2]
    pextrw  eax, xmm2, 1      ; eax <- in_out[2] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- LS_mat[0][in_out[0]] ^ LS_mat[1][in_out[1]] ^ LS_mat[2][in_out[2]]
    add r11, 4096           ; r11 <- LS_mat[3]
    pextrw  eax, xmm3, 1      ; eax <- in_out[3] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[3][in_out[3]]
    add r11, 4096           ; r11 <- LS_mat[4]
    pextrw  eax, xmm2, 2      ; eax <- in_out[4] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[4][in_out[4]]
    add r11, 4096           ; r11 <- LS_mat[5]
    pextrw  eax, xmm3, 2      ; eax <- in_out[5] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[5][in_out[5]]
    add r11, 4096           ; r11 <- LS_mat[6]
    pextrw  eax, xmm2, 3      ; eax <- in_out[6] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[6][in_out[6]]
    add r11, 4096           ; r11 <- LS_mat[7]
    pextrw  eax, xmm3, 3      ; eax <- in_out[7] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[7][in_out[7]]
    add r11, 4096           ; r11 <- LS_mat[8]
    pextrw  eax, xmm2, 4      ; eax <- in_out[8] * 16
    pxor xmm1, [r11 + rax]; xmm1 <- LS_mat[8][in_out[8]]
    add r11, 4096           ; r11 <- LS_mat[9]
    pextrw  eax, xmm3, 4      ; eax <- in_out[9] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- LS_mat[8][in_out[8]] ^ LS_mat[9][in_out[9]]
    add r11, 4096           ; r11 <- LS_mat[10]
    pextrw  eax, xmm2, 5     ; eax <- in_out[10] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[10][in_out[10]]
    add r11, 4096           ; r11 <- LS_mat[11]
    pextrw  eax, xmm3, 5     ; eax <- in_out[11] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[11][in_out[11]]
    add r11, 4096           ; r11 <- LS_mat[12]
    pextrw  eax, xmm2, 6     ; eax <- in_out[12] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[12][in_out[12]]
    add r11, 4096           ; r11 <- LS_mat[13]
    pextrw  eax, xmm3, 6     ; eax <- in_out[13] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[13][in_out[13]]
    add r11, 4096           ; r11 <- LS_mat[14]
    pextrw  eax, xmm2, 7     ; eax <- in_out[14] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[14][in_out[14]]
    add r11, 4096           ; r11 <- LS_mat[15]
    pextrw  eax, xmm3, 7     ; eax <- in_out[15] * 16
    pxor xmm1, [r11 + rax]  ; xmm1 <- ... ^ LS_mat[15][in_out[15]]
    movaps [rcx], xmm1      ; in_out <- xmm1
    ret

; Arguments:
;   rcx - round_keys (chunk *) - a pointer to the round keys
;   rdx - in (chunk) - a pointer to the input data
;   r8 - out (chunk) - a pointer to the output data
kyznechick_asm:
    movaps xmm0, [rdx]      ; xmm0 <- in
    pxor xmm0, [rcx]        ; xmm0 <- in ^ round_keys


