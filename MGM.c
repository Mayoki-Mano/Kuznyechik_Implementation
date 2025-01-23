#define BLOCK_SIZE 16
#define NONCE_SIZE 16
#define T_SIZE 16
#include <stdint.h>
#include "Kuznechik.h"
// Функция для преобразования числа из Little Endian в Big Endian
uint64_t to_big_endian(uint64_t little_endian_value) {
    return ((little_endian_value >> 56) & 0xFF) |
           ((little_endian_value >> 40) & 0xFF00) |
           ((little_endian_value >> 24) & 0xFF0000) |
           ((little_endian_value >> 8) & 0xFF000000) |
           ((little_endian_value << 8) & 0xFF00000000) |
           ((little_endian_value << 24) & 0xFF0000000000) |
           ((little_endian_value << 40) & 0xFF000000000000) |
           ((little_endian_value << 56) & 0xFF00000000000000);
}
void chunk_swap_endian(chunk p) {
    p[0]=to_big_endian(p[0]);
    p[1]=to_big_endian(p[1]);
}



// Функция умножения в поле Галуа (2^128)
void GF_mult128_chunk(const chunk a, const chunk b, chunk result) {
    chunk c = {0, 0}; // Результат умножения
    chunk temp_a = {a[0], a[1]}; // Копия числа a
    chunk temp_b = {b[0], b[1]}; // Копия числа b
    chunk_swap_endian(temp_a);
    chunk_swap_endian(temp_b);

    // Полином редукции x^128 + x^7 + x^2 + x + 1
    const chunk gf_128_mod = {0x0000000000000000,0x87 };

    // Цикл по всем битам числа b
    while (temp_b[0] || temp_b[1]) {
        // Если младший бит temp_b равен 1, добавляем temp_a в результат
        if (temp_b[1] & 1) {
            c[0] ^= temp_a[0];
            c[1] ^= temp_a[1];
        }

        // Сдвигаем temp_a влево
        uint64_t carry = (temp_a[0] & 0x8000000000000000) ? 1 : 0; // Перенос из старшей части
        temp_a[0] = (temp_a[0] << 1) | (temp_a[1] >> 63); // Перенос между частями
        temp_a[1] = (temp_a[1] << 1);

        // Если был перенос, выполняем редукцию
        if (carry) {
            temp_a[0] ^= gf_128_mod[0];
            temp_a[1] ^= gf_128_mod[1];
        }

        // Сдвигаем temp_b вправо
        temp_b[1] = (temp_b[1] >> 1) | (temp_b[0] << 63); // Перенос между частями
        temp_b[0] = (temp_b[0] >> 1);


    }

    // Записываем результат
    result[0] = c[0];
    result[1] = c[1];
    chunk_swap_endian(result);
}



// Функция инкрементации в Big Endian
void increment_in_big_endian(uint64_t *num) {
    // Преобразуем число в Big Endian
    uint64_t big_endian = to_big_endian(*num);

    // Инкрементируем число
    big_endian += 1;

    // Преобразуем обратно в Little Endian
    *num = to_big_endian(big_endian);
}

void MGM_decrypt(uint8_t *key, uint8_t *nonce, uint8_t *P, uint8_t *C, uint8_t *A, int C_len, int A_len, uint8_t *res_T,
                 int T_len) {
    // Итерационные ключи
    chunk round_keys[10] = {};
    // Генерация итерационных ключей
    gen_round_keys(key, round_keys);
    chunk Y, Z, H, T, G; //G - gamma for encryption
    chunk P_last = {0}, A_last = {0}, C_last = {0};
    T[0] = 0;
    T[1] = 0;
    int i;
    int h, q;
    if (A_len % BLOCK_SIZE) {
        h = A_len / BLOCK_SIZE + 1;
        memcpy(A_last, A + (h - 1) * BLOCK_SIZE, A_len % BLOCK_SIZE);
    } else {
        h = A_len / BLOCK_SIZE;
        memcpy(A_last, A + (h - 1) * BLOCK_SIZE, BLOCK_SIZE);
    }
    if (C_len % BLOCK_SIZE) {
        q = C_len / BLOCK_SIZE + 1;
        memcpy(C_last, C + (q - 1) * BLOCK_SIZE, C_len % BLOCK_SIZE);
    } else {
        q = C_len / BLOCK_SIZE;
        memcpy(C_last, C + (q - 1) * BLOCK_SIZE, BLOCK_SIZE);
    }
    nonce[0] &= 0x7F;
    kuznechik_encrypt(round_keys, (void *) nonce, Y);
    nonce[0] |= 0x80;
    kuznechik_encrypt(round_keys, (void *) nonce, Z);
    for (i = 0; i < h; ++i) {
        kuznechik_encrypt(round_keys, (void *) Z, H);
        increment_in_big_endian(&Z[0]);
        if (i == h - 1)
            GF_mult128_chunk(H, A_last, H);
        else
            GF_mult128_chunk(H, (void *) A + i * BLOCK_SIZE, H);
        X(T, H, T);
    }
    for (i = 0; i < q; ++i) {
        kuznechik_encrypt(round_keys, (void *) Z, H);
        increment_in_big_endian(&Z[0]);
        if (i == q - 1)
            GF_mult128_chunk(H, C_last, H);
        else
            GF_mult128_chunk(H, (void *) C + i * BLOCK_SIZE, H);
        X(T, H, T);
    }
    kuznechik_encrypt(round_keys, (void *) Z, H);
    G[0] = to_big_endian(A_len * 8);
    G[1] = to_big_endian(C_len * 8);
    GF_mult128_chunk(H, G, H);
    X(T, H, T);
    kuznechik_encrypt(round_keys, T, T);
    // if (memcmp(T, res_T, T_len) != 0) {
    //     printf("Error T is not correct\n");
    //     return;
    // }
    for (i = 0; i < q; ++i) {
        kuznechik_encrypt(round_keys, (void *) Y, G);
        if (i == q - 1) {
            memcpy(C_last, G, C_len % BLOCK_SIZE);
            X(P_last, C_last, P_last);
            memcpy((void *) P + i * BLOCK_SIZE, P_last, C_len % BLOCK_SIZE);
        } else {
            increment_in_big_endian(&Y[1]);
            X((void *) C + i * BLOCK_SIZE, G, (void *) P + i * BLOCK_SIZE);
        }
    }
}



void MGM_encrypt(uint8_t *key, uint8_t *nonce, uint8_t *P, uint8_t *C, uint8_t *A, int P_len, int A_len, uint8_t *res_T,
                 int T_len) {
    // Итерационные ключи
    chunk round_keys[10] = {};
    // Генерация итерационных ключей
    gen_round_keys(key, round_keys);
    chunk Y, Z, H, T, G; //G - gamma for encryption
    chunk P_last = {0}, A_last = {0}, C_last = {0};
    T[0] = 0;
    T[1] = 0;
    int i;
    int h, q;
    if (A_len % BLOCK_SIZE) {
        h = A_len / BLOCK_SIZE + 1;
        memcpy(A_last, A + (h - 1) * BLOCK_SIZE, A_len % BLOCK_SIZE);
    } else {
        h = A_len / BLOCK_SIZE;
        memcpy(A_last, A + (h - 1) * BLOCK_SIZE, BLOCK_SIZE);
    }
    if (P_len % BLOCK_SIZE) {
        q = P_len / BLOCK_SIZE + 1;
        memcpy(P_last, P + (q - 1) * BLOCK_SIZE, P_len % BLOCK_SIZE);
    } else {
        q = P_len / BLOCK_SIZE;
        memcpy(P_last, P + (q - 1) * BLOCK_SIZE, BLOCK_SIZE);
    }
    nonce[0] &= 0x7F;
    kuznechik_encrypt(round_keys, (void *) nonce, Y);
    nonce[0] |= 0x80;
    kuznechik_encrypt(round_keys, (void *) nonce, Z);
    for (i = 0; i < h; ++i) {
        kuznechik_encrypt(round_keys, (void *) Z, H);
        increment_in_big_endian(&Z[0]);
        if (i == h - 1)
            GF_mult128_chunk(H, A_last, H);
        else
            GF_mult128_chunk(H, (void *) A + i * BLOCK_SIZE, H);
        X(T, H, T);
    }
    for (i = 0; i < q; ++i) {
        kuznechik_encrypt(round_keys, (void *) Z, H);
        increment_in_big_endian(&Z[0]);
        kuznechik_encrypt(round_keys, (void *) Y, G);
        if (i == q - 1) {
            memcpy(C_last, G, P_len % BLOCK_SIZE);
            X(C_last, P_last, C_last);
            memcpy((void *) C + i * BLOCK_SIZE, C_last, P_len % BLOCK_SIZE);
            GF_mult128_chunk(H, C_last, H);
        } else {
            increment_in_big_endian(&Y[1]);
            X((void *) P + i * BLOCK_SIZE, G, (void *) C + i * BLOCK_SIZE);
            GF_mult128_chunk(H, (void *) C + i * BLOCK_SIZE, H);
        }
        X(T, H, T);
    }
    kuznechik_encrypt(round_keys, (void *) Z, H);
    G[0] = to_big_endian(A_len * 8);
    G[1] = to_big_endian(P_len * 8);
    GF_mult128_chunk(H, G, H);
    X(T, H, T);
    kuznechik_encrypt(round_keys, T, T);
    memcpy(res_T, T, T_len);
}

void tests() {
    generate_LUT();
    uint8_t key[] = {
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
    };
    chunk round_keys[10] = {0};
    chunk round_keys_L_reversed[10] = {0};
    gen_round_keys(key, round_keys);
    memcpy(round_keys_L_reversed, round_keys, sizeof(round_keys));
    for (int i=1;i<10;++i)
        L_reverse((void *)round_keys_L_reversed[i]);
    printf("\n");
    uint8_t data[KUZNECHIK_BLOCK_SIZE] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00,
        0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88
    };
    printf("Plain text:\n");
    print_chunk((void *) data);
    chunk encrypted;
    memcpy(encrypted, data, sizeof(chunk));

    int enc_dec_times = 10000000;
    clock_t start_time = clock();
    for (int i = 0; i < enc_dec_times; i++) {
        kuznechik_encrypt(round_keys, (void *) encrypted, encrypted);
    }
    clock_t end_time = clock();
    double elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Encoded text:\n");
    print_chunk(encrypted);
    printf("Encryption kuznechik speed: %.6f MB/sec\n", (enc_dec_times*KUZNECHIK_BLOCK_SIZE)/elapsed_time/1024/1024);
    start_time = clock();
    for (int i = 0; i < enc_dec_times; i++) {
        kuznechik_decrypt(round_keys_L_reversed, (void *) encrypted, encrypted);
    }
    end_time = clock();
    elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Decoded text:\n");
    print_chunk(encrypted);
    printf("Decryption kuznechik speed: %.6f MB/sec\n",  (enc_dec_times*KUZNECHIK_BLOCK_SIZE)/elapsed_time/1024/1024);

    uint8_t nonce[] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00, 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88
    };
    uint8_t A[] = {
        0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0xea, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05
    };
    uint8_t P[] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00, 0xFF, 0xee, 0xDD, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xee, 0xff, 0x0a,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xee, 0xff, 0x0a, 0x00,
        0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xee, 0xff, 0x0a, 0x00, 0x11,
        0xaa, 0xbb, 0xcc
    };
    uint8_t C[sizeof(P)];
    uint8_t T[T_SIZE];
    MGM_encrypt(key, nonce, P, C, A, sizeof(P), sizeof(A), T, sizeof(T));
    printf("T: ");
    print(T,T_SIZE);
    printf("\nCiphertext:\n");
    print(C, sizeof(P));
    uint8_t test_chunk1[16] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t test_chunk2[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};
    chunk res;
    GF_mult128_chunk((void *)test_chunk1, (void *)test_chunk2, res);
    MGM_decrypt(key, nonce, P, C, A, sizeof(C), sizeof(A), T, sizeof(T));
    printf("\nPlaintext:\n");
    print(P, sizeof(P));
    enc_dec_times = 100000;
    start_time = clock();
    for (int i = 0; i < enc_dec_times; i++) {
        MGM_encrypt(key, nonce, P, P, A, sizeof(P), sizeof(A), T, sizeof(T));
    }
    end_time = clock();
    elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Encoded text:\n");
    print(P, sizeof(P));
    printf("Encryption MGM speed: %.6f MB/sec\n", (enc_dec_times*KUZNECHIK_BLOCK_SIZE*8)/elapsed_time/1024/1024);
    start_time = clock();
    for (int i = 0; i < enc_dec_times; i++) {
        MGM_decrypt(key, nonce, P, P, A, sizeof(P), sizeof(A), T, sizeof(T));
    }
    end_time = clock();
    elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Decoded text:\n");
    print(P, sizeof(P));
    printf("Decryption MGM speed: %.6f MB/sec\n",  (enc_dec_times*KUZNECHIK_BLOCK_SIZE*8)/elapsed_time/1024/1024);
}




int main() {
    tests();
}
