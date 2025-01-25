section .data
mask db 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF

section .bss

section .text
global LS_asm                   ; Объявляем функцию LS как глобальную
global kyznechick_asm_152
global kyznechick_asm_fast
global kyznechick_asm_133

extern LS_mat               ; Объявляем глобальный массив LS_mat
; used- r9,r11,r8,rcx,rax
; unused - r10, rdx, rcx(?)
; требуют восстановления, но в принципе можно использовать rbx, rbp(?), rdi, rsi, r12, r13, r14, r15
; Arguments:
;   rcx - round_keys (chunk *) - a pointer to the round keys
;   rdx - in (chunk) - a pointer to the input data
;   r8 - out (chunk) - a pointer to the output data
kyznechick_asm_152:
    push r12
    push r13
    push r14
    push r15
    push rbx
    push rbp
    push rsi
    push rdi
    ; Загрузка входных данных (16 байт)
    movaps xmm0, [rdx]      ; xmm0 <- in
    mov r11, LS_mat         ; r11 <- LS_mat
    mov r10, r11            ; r10 <- LS_mat
    add r11, 4096             ; r11 <- LS_mat[1]
    mov rdx, r11            ; rdx <- LS_mat[1]
    add r11, 4096             ; r11 <- LS_mat[2]
    mov r12, r11            ; r12 <- LS_mat[2]
    add r11, 4096             ; r11 <- LS_mat[3]
    mov r13, r11            ; r13 <- LS_mat[3]
    add r11, 4096             ; r11 <- LS_mat[4]
    mov r14, r11            ; r14 <- LS_mat[4]
    add r11, 4096             ; r11 <- LS_mat[5]
    mov r15, r11            ; r15 <- LS_mat[5]
    add r11, 4096             ; r11 <- LS_mat[6]
    mov rsi, r11            ; rsi <- LS_mat[6]
    add r11, 4096             ; r11 <- LS_mat[7]
    mov rdi, r11            ; rdi <- LS_mat[7]
    add r11, 4096             ; r11 <- LS_mat[8]
    mov rbp, r11            ; rbp <- LS_mat[8]
    add r11, 4096             ; r11 <- LS_mat[9]
    mov rbx, r11            ; rbx <- LS_mat[9]
    add r11, 4096             ; r11 <- LS_mat[10]
    lea r9, [rel mask]      ; r9 <- &mask
    movaps xmm5, [r9]       ; xmm5 <- mask
    mov r9, rcx             ; r9 <- указатель на round_keys
    ; Основной цикл раундов (9 итераций)
    mov ecx, 9              ; Количество раундов (9 для "Кузнечика")


round_loop:
    ; Применение раундового ключа
    pxor xmm0, [r9]         ; xmm0 <- in ^ round_keys[i]
    add r9, 16              ; Переход к следующему раундовому ключу

    ; Разделение байтов на чётные и нечётные
    movaps xmm3, xmm5       ; xmm3 <- mask
    movaps xmm2, xmm5       ; xmm2 <- mask
    pandn xmm2, xmm0        ; xmm2 <- чётные байты in
    pand xmm3, xmm0         ; xmm3 <- нечётные байты in
    psllw xmm2, 4           ; xmm2 <- умножение чётных байтов на 16
    psrlw xmm3, 4           ; xmm3 <- умножение нечётных байтов на 16

    ; Обработка всех 16 байтов (развёрнутый цикл)
    ; Байт 0 (чётный)
    pextrw eax, xmm2, 0      ; eax <- in_out[0] * 16
    movaps xmm0, [r10 + rax]         ; xmm0 <- xmm0 ^ LS_mat[0][in_out[0]]

    ; Байт 1 (нечётный)
    pextrw eax, xmm3, 0      ; eax <- in_out[1] * 16
    pxor xmm0, [rdx+rax]         ; xmm0 <- xmm0 ^ LS_mat[1][in_out[1]]

    ; Байт 2 (чётный)
    pextrw eax, xmm2, 1      ; eax <- in_out[2] * 16
    pxor xmm0, [r12+rax]         ; xmm0 <- xmm0 ^ LS_mat[2][in_out[2]]

    ; Байт 3 (нечётный)
    pextrw eax, xmm3, 1      ; eax <- in_out[3] * 16
    pxor xmm0, [r13+rax]         ; xmm0 <- xmm0 ^ LS_mat[3][in_out[3]]

    ; Байт 4 (чётный)
    pextrw eax, xmm2, 2      ; eax <- in_out[4] * 16
    pxor xmm0, [r14+rax]         ; xmm0 <- xmm0 ^ LS_mat[4][in_out[4]]

    ; Байт 5 (нечётный)
    pextrw eax, xmm3, 2      ; eax <- in_out[5] * 16
    pxor xmm0, [r15+rax]         ; xmm0 <- xmm0 ^ LS_mat[5][in_out[5]]

    ; Байт 6 (чётный)
    pextrw eax, xmm2, 3      ; eax <- in_out[6] * 16
    pxor xmm0, [rsi+rax]         ; xmm0 <- xmm0 ^ LS_mat[6][in_out[6]]

    ; Байт 7 (нечётный)
    pextrw eax, xmm3, 3      ; eax <- in_out[7] * 16
    pxor xmm0, [rdi+rax]         ; xmm0 <- xmm0 ^ LS_mat[7][in_out[7]]

    ; Байт 8 (чётный)
    pextrw eax, xmm2, 4      ; eax <- in_out[8] * 16
    pxor xmm0, [rbp+rax]         ; xmm0 <- xmm0 ^ LS_mat[8][in_out[8]]

    ; Байт 9 (нечётный)
    pextrw eax, xmm3, 4      ; eax <- in_out[9] * 16
    pxor xmm0, [rbx+rax]         ; xmm0 <- xmm0 ^ LS_mat[9][in_out[9]]

    ; Байт 10 (чётный)
    pextrw eax, xmm2, 5      ; eax <- in_out[10] * 16
    pxor xmm0, [r11+rax]         ; xmm0 <- xmm0 ^ LS_mat[10][in_out[10]]

    ; Байт 11 (нечётный)
    pextrw eax, xmm3, 5      ; eax <- in_out[11] * 16
    add r11, 4096
    pxor xmm0, [r11+rax]         ; xmm0 <- xmm0 ^ LS_mat[11][in_out[11]]

    ; Байт 12 (чётный)
    pextrw eax, xmm2, 6      ; eax <- in_out[12] * 16
    add r11, 4096
    pxor xmm0, [r11+rax]         ; xmm0 <- xmm0 ^ LS_mat[12][in_out[12]]

    ; Байт 13 (нечётный)
    pextrw eax, xmm3, 6      ; eax <- in_out[13] * 16
    add r11, 4096
    pxor xmm0, [r11+rax]         ; xmm0 <- xmm0 ^ LS_mat[13][in_out[13]]

    ; Байт 14 (чётный)
    pextrw eax, xmm2, 7      ; eax <- in_out[14] * 16
    add r11, 4096
    pxor xmm0, [r11+rax]         ; xmm0 <- xmm0 ^ LS_mat[14][in_out[14]]

    ; Байт 15 (нечётный)
    pextrw eax, xmm3, 7      ; eax <- in_out[15] * 16
    add r11, 4096
    pxor xmm0, [r11+rax]         ; xmm0 <- xmm0 ^ LS_mat[15][in_out[15]]

    sub r11, 20480          ; r11 <- LS_mat
    ; Следующий раунд
    dec ecx                 ; Уменьшаем счётчик раундов
    jnz round_loop          ; Если ecx != 0, продолжаем цикл

    ; Финальный раунд (последний XOR с раундовым ключом)
    pxor xmm0, [r9]         ; xmm0 <- in ^ round_keys[9]
    movaps [r8], xmm0      ; Сохранение результата в out
    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12
    ret

; Arguments:
;   rcx - round_keys (chunk *) - a pointer to the round keys
;   rdx - in (chunk) - a pointer to the input data
;   r8 - out (chunk) - a pointer to the output data
kyznechick_asm_133:
    ; Загрузка входных данных (16 байт)
    movdqa xmm1, [rdx]      ; xmm1 <- in (16 байт)
    mov r11, LS_mat          ; r11 <- указатель на LUT-таблицу LS_mat
    ; Основной цикл раундов
    mov r9, rcx              ; r9 <- указатель на round_keys
    mov ecx, 9               ; Количество раундов (9 для "Кузнечика")

round_loop2:
    ; Применение раундового ключа
    pxor xmm1, [r9]   ; xmm1 <- in ^ round_keys[i]
    add r9, 16               ; Переход к следующему раундовому ключу

    ; Инициализация результата нулями
    pxor xmm0, xmm0   ; xmm0 <- 0 (результат)

    ; Нелинейное преобразование через LUT-таблицу
    ; Развёрнутый цикл для обработки каждого байта (0..15)
    ; Байт 0
    pextrb eax, xmm1, 0      ; eax <- in_out[0]
    shl eax, 4               ; eax <- in_out[0] * 16
    lea r12, [r11 + rax]     ; r12 <- LS_mat[0][in_out[0]]
    movdqa xmm2, [r11 + rax]      ; xmm2 <- LS_mat[0][in_out[0]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[0][in_out[0]]

    ; Байт 1
    pextrb eax, xmm1, 1      ; eax <- in_out[1]
    shl eax, 4               ; eax <- in_out[1] * 16
    lea r12, [r11 + 4096 + rax] ; r12 <- LS_mat[1][in_out[1]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[1][in_out[1]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[1][in_out[1]]

    ; Байт 2
    pextrb eax, xmm1, 2      ; eax <- in_out[2]
    shl eax, 4               ; eax <- in_out[2] * 16
    lea r12, [r11 + 8192 + rax] ; r12 <- LS_mat[2][in_out[2]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[2][in_out[2]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[2][in_out[2]]

    ; Байт 3
    pextrb eax, xmm1, 3      ; eax <- in_out[3]
    shl eax, 4               ; eax <- in_out[3] * 16
    lea r12, [r11 + 12288 + rax] ; r12 <- LS_mat[3][in_out[3]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[3][in_out[3]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[3][in_out[3]]

    ; Байт 4
    pextrb eax, xmm1, 4      ; eax <- in_out[4]
    shl eax, 4               ; eax <- in_out[4] * 16
    lea r12, [r11 + 16384 + rax] ; r12 <- LS_mat[4][in_out[4]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[4][in_out[4]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[4][in_out[4]]

    ; Байт 5
    pextrb eax, xmm1, 5      ; eax <- in_out[5]
    shl eax, 4               ; eax <- in_out[5] * 16
    lea r12, [r11 + 20480 + rax] ; r12 <- LS_mat[5][in_out[5]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[5][in_out[5]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[5][in_out[5]]

    ; Байт 6
    pextrb eax, xmm1, 6      ; eax <- in_out[6]
    shl eax, 4               ; eax <- in_out[6] * 16
    lea r12, [r11 + 24576 + rax] ; r12 <- LS_mat[6][in_out[6]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[6][in_out[6]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[6][in_out[6]]

    ; Байт 7
    pextrb eax, xmm1, 7      ; eax <- in_out[7]
    shl eax, 4               ; eax <- in_out[7] * 16
    lea r12, [r11 + 28672 + rax] ; r12 <- LS_mat[7][in_out[7]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[7][in_out[7]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[7][in_out[7]]

    ; Байт 8
    pextrb eax, xmm1, 8      ; eax <- in_out[8]
    shl eax, 4               ; eax <- in_out[8] * 16
    lea r12, [r11 + 32768 + rax] ; r12 <- LS_mat[8][in_out[8]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[8][in_out[8]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[8][in_out[8]]

    ; Байт 9
    pextrb eax, xmm1, 9      ; eax <- in_out[9]
    shl eax, 4               ; eax <- in_out[9] * 16
    lea r12, [r11 + 36864 + rax] ; r12 <- LS_mat[9][in_out[9]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[9][in_out[9]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[9][in_out[9]]

    ; Байт 10
    pextrb eax, xmm1, 10     ; eax <- in_out[10]
    shl eax, 4               ; eax <- in_out[10] * 16
    lea r12, [r11 + 40960 + rax] ; r12 <- LS_mat[10][in_out[10]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[10][in_out[10]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[10][in_out[10]]

    ; Байт 11
    pextrb eax, xmm1, 11     ; eax <- in_out[11]
    shl eax, 4               ; eax <- in_out[11] * 16
    lea r12, [r11 + 45056 + rax] ; r12 <- LS_mat[11][in_out[11]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[11][in_out[11]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[11][in_out[11]]

    ; Байт 12
    pextrb eax, xmm1, 12     ; eax <- in_out[12]
    shl eax, 4               ; eax <- in_out[12] * 16
    lea r12, [r11 + 49152 + rax] ; r12 <- LS_mat[12][in_out[12]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[12][in_out[12]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[12][in_out[12]]

    ; Байт 13
    pextrb eax, xmm1, 13     ; eax <- in_out[13]
    shl eax, 4               ; eax <- in_out[13] * 16
    lea r12, [r11 + 53248 + rax] ; r12 <- LS_mat[13][in_out[13]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[13][in_out[13]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[13][in_out[13]]

    ; Байт 14
    pextrb eax, xmm1, 14     ; eax <- in_out[14]
    shl eax, 4               ; eax <- in_out[14] * 16
    lea r12, [r11 + 57344 + rax] ; r12 <- LS_mat[14][in_out[14]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[14][in_out[14]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[14][in_out[14]]

    ; Байт 15
    pextrb eax, xmm1, 15     ; eax <- in_out[15]
    shl eax, 4               ; eax <- in_out[15] * 16
    lea r12, [r11 + 61440 + rax] ; r12 <- LS_mat[15][in_out[15]]
    movdqa xmm2, [r12]      ; xmm2 <- LS_mat[15][in_out[15]]
    pxor xmm0, xmm2   ; xmm0 <- xmm0 ^ LS_mat[15][in_out[15]]

    movdqa xmm1, xmm0       ; xmm1 <- результат
    ; Следующий раунд
    dec ecx                  ; Уменьшаем счётчик раундов
    jnz round_loop2           ; Если ecx != 0, продолжаем цикл

    ; Финальный раунд
    pxor xmm0, [r9]   ; xmm1 <- in ^ round_keys[9]
    movdqa [r8], xmm0       ; Сохранение результата в out

    ret


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