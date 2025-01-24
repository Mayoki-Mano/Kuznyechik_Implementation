
#ifndef KUZNECHIK_H
#define KUZNECHIK_H
#include <stdio.h>
#include <emmintrin.h> // SSE2
// uint8_t, uint64_t
#include <stdint.h>
// memcpy
#include <string.h>
#include <time.h>
#include <immintrin.h> // Для инструкций AVX
// Один блок (чанк) задается как массив двух беззнаковых целых по 64 бита
typedef uint64_t chunk[2];
#define FIELD_SIZE 256
// Длинна блока в байтах(16 байт = 128 бит)
#define KUZNECHIK_BLOCK_SIZE 16


uint8_t GF_mult8(uint8_t a, uint8_t b);
void square_matrix(uint8_t *Z);
void fill_galois_multiplication_table(uint8_t table[FIELD_SIZE][FIELD_SIZE]);
void generate_LUT();
void X(const chunk a, const chunk b, chunk c);
void LS(uint8_t *in_out);
void LS_reverse(uint8_t *in_out);
void S(chunk in_out);
void S_reverse(chunk in_out);
void R(uint8_t *in_out);
void R_reverse(uint8_t *in_out);
void L(uint8_t *in_out);
void L_reverse(uint8_t *in_out);
void gen_round_keys(const uint8_t *key, chunk *round_keys);
void kuznechik_encrypt(chunk *round_keys, chunk in, chunk out);
void kuznechik_decrypt(chunk *round_keys, chunk in, chunk out);
void print_chunk(chunk p);
void print(uint8_t *p, int size);
void aligned_load_array_to_L1(int *arr, size_t size);
void test_asm();
extern void kyznechick_asm_152(chunk *round_keys, chunk in, chunk out);
extern void kyznechick_asm_fast(chunk *round_keys, chunk in, chunk out);
extern void kyznechick_asm_133(chunk *round_keys, chunk in, chunk out);


#endif //KUZNECHIK_H
