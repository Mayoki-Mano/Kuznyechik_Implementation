
#include "Kuznechik.h"



// Таблица прямого нелинейного преобразования согластно ГОСТ 34.12-2015
const uint8_t Pi[] = {
    0xFC, 0xEE, 0xDD, 0x11, 0xCF, 0x6E, 0x31, 0x16, 0xFB, 0xC4, 0xFA, 0xDA, 0x23, 0xC5, 0x04, 0x4D,
    0xE9, 0x77, 0xF0, 0xDB, 0x93, 0x2E, 0x99, 0xBA, 0x17, 0x36, 0xF1, 0xBB, 0x14, 0xCD, 0x5F, 0xC1,
    0xF9, 0x18, 0x65, 0x5A, 0xE2, 0x5C, 0xEF, 0x21, 0x81, 0x1C, 0x3C, 0x42, 0x8B, 0x01, 0x8E, 0x4F,
    0x05, 0x84, 0x02, 0xAE, 0xE3, 0x6A, 0x8F, 0xA0, 0x06, 0x0B, 0xED, 0x98, 0x7F, 0xD4, 0xD3, 0x1F,
    0xEB, 0x34, 0x2C, 0x51, 0xEA, 0xC8, 0x48, 0xAB, 0xF2, 0x2A, 0x68, 0xA2, 0xFD, 0x3A, 0xCE, 0xCC,
    0xB5, 0x70, 0x0E, 0x56, 0x08, 0x0C, 0x76, 0x12, 0xBF, 0x72, 0x13, 0x47, 0x9C, 0xB7, 0x5D, 0x87,
    0x15, 0xA1, 0x96, 0x29, 0x10, 0x7B, 0x9A, 0xC7, 0xF3, 0x91, 0x78, 0x6F, 0x9D, 0x9E, 0xB2, 0xB1,
    0x32, 0x75, 0x19, 0x3D, 0xFF, 0x35, 0x8A, 0x7E, 0x6D, 0x54, 0xC6, 0x80, 0xC3, 0xBD, 0x0D, 0x57,
    0xDF, 0xF5, 0x24, 0xA9, 0x3E, 0xA8, 0x43, 0xC9, 0xD7, 0x79, 0xD6, 0xF6, 0x7C, 0x22, 0xB9, 0x03,
    0xE0, 0x0F, 0xEC, 0xDE, 0x7A, 0x94, 0xB0, 0xBC, 0xDC, 0xE8, 0x28, 0x50, 0x4E, 0x33, 0x0A, 0x4A,
    0xA7, 0x97, 0x60, 0x73, 0x1E, 0x00, 0x62, 0x44, 0x1A, 0xB8, 0x38, 0x82, 0x64, 0x9F, 0x26, 0x41,
    0xAD, 0x45, 0x46, 0x92, 0x27, 0x5E, 0x55, 0x2F, 0x8C, 0xA3, 0xA5, 0x7D, 0x69, 0xD5, 0x95, 0x3B,
    0x07, 0x58, 0xB3, 0x40, 0x86, 0xAC, 0x1D, 0xF7, 0x30, 0x37, 0x6B, 0xE4, 0x88, 0xD9, 0xE7, 0x89,
    0xE1, 0x1B, 0x83, 0x49, 0x4C, 0x3F, 0xF8, 0xFE, 0x8D, 0x53, 0xAA, 0x90, 0xCA, 0xD8, 0x85, 0x61,
    0x20, 0x71, 0x67, 0xA4, 0x2D, 0x2B, 0x09, 0x5B, 0xCB, 0x9B, 0x25, 0xD0, 0xBE, 0xE5, 0x6C, 0x52,
    0x59, 0xA6, 0x74, 0xD2, 0xE6, 0xF4, 0xB4, 0xC0, 0xD1, 0x66, 0xAF, 0xC2, 0x39, 0x4B, 0x63, 0xB6
};

// Таблица обратного нелинейного преобразования
static const uint8_t Pi_reverse[] = {
    0xA5, 0x2D, 0x32, 0x8F, 0x0E, 0x30, 0x38, 0xC0, 0x54, 0xE6, 0x9E, 0x39, 0x55, 0x7E, 0x52, 0x91,
    0x64, 0x03, 0x57, 0x5A, 0x1C, 0x60, 0x07, 0x18, 0x21, 0x72, 0xA8, 0xD1, 0x29, 0xC6, 0xA4, 0x3F,
    0xE0, 0x27, 0x8D, 0x0C, 0x82, 0xEA, 0xAE, 0xB4, 0x9A, 0x63, 0x49, 0xE5, 0x42, 0xE4, 0x15, 0xB7,
    0xC8, 0x06, 0x70, 0x9D, 0x41, 0x75, 0x19, 0xC9, 0xAA, 0xFC, 0x4D, 0xBF, 0x2A, 0x73, 0x84, 0xD5,
    0xC3, 0xAF, 0x2B, 0x86, 0xA7, 0xB1, 0xB2, 0x5B, 0x46, 0xD3, 0x9F, 0xFD, 0xD4, 0x0F, 0x9C, 0x2F,
    0x9B, 0x43, 0xEF, 0xD9, 0x79, 0xB6, 0x53, 0x7F, 0xC1, 0xF0, 0x23, 0xE7, 0x25, 0x5E, 0xB5, 0x1E,
    0xA2, 0xDF, 0xA6, 0xFE, 0xAC, 0x22, 0xF9, 0xE2, 0x4A, 0xBC, 0x35, 0xCA, 0xEE, 0x78, 0x05, 0x6B,
    0x51, 0xE1, 0x59, 0xA3, 0xF2, 0x71, 0x56, 0x11, 0x6A, 0x89, 0x94, 0x65, 0x8C, 0xBB, 0x77, 0x3C,
    0x7B, 0x28, 0xAB, 0xD2, 0x31, 0xDE, 0xC4, 0x5F, 0xCC, 0xCF, 0x76, 0x2C, 0xB8, 0xD8, 0x2E, 0x36,
    0xDB, 0x69, 0xB3, 0x14, 0x95, 0xBE, 0x62, 0xA1, 0x3B, 0x16, 0x66, 0xE9, 0x5C, 0x6C, 0x6D, 0xAD,
    0x37, 0x61, 0x4B, 0xB9, 0xE3, 0xBA, 0xF1, 0xA0, 0x85, 0x83, 0xDA, 0x47, 0xC5, 0xB0, 0x33, 0xFA,
    0x96, 0x6F, 0x6E, 0xC2, 0xF6, 0x50, 0xFF, 0x5D, 0xA9, 0x8E, 0x17, 0x1B, 0x97, 0x7D, 0xEC, 0x58,
    0xF7, 0x1F, 0xFB, 0x7C, 0x09, 0x0D, 0x7A, 0x67, 0x45, 0x87, 0xDC, 0xE8, 0x4F, 0x1D, 0x4E, 0x04,
    0xEB, 0xF8, 0xF3, 0x3E, 0x3D, 0xBD, 0x8A, 0x88, 0xDD, 0xCD, 0x0B, 0x13, 0x98, 0x02, 0x93, 0x80,
    0x90, 0xD0, 0x24, 0x34, 0xCB, 0xED, 0xF4, 0xCE, 0x99, 0x10, 0x44, 0x40, 0x92, 0x3A, 0x01, 0x26,
    0x12, 0x1A, 0x48, 0x68, 0xF5, 0x81, 0x8B, 0xC7, 0xD6, 0x20, 0x0A, 0x08, 0x00, 0x4C, 0xD7, 0x74
};

// Вектор линейного преобразования
const uint8_t linear_vector[] = {
    0x94, 0x20, 0x85, 0x10, 0xC2, 0xC0, 0x01, 0xFB,
    0x01, 0xC0, 0xC2, 0x10, 0x85, 0x20, 0x94, 0x01
};
_Alignas(16) uint8_t LS_mat[16][256][16] ={0};


uint8_t LS_reverse_mat[16][256][16] = {0};
uint8_t Mul[FIELD_SIZE][FIELD_SIZE] = {0};

// Функция умножения в поле Галуа(2^8)
uint8_t GF_mult8(uint8_t a, uint8_t b) {
    uint8_t c;

    c = 0;
    while (b) {
        if (b & 1)
            c ^= a;
        a = (a << 1) ^ (a & 0x80 ? 0xC3 : 0x00);
        b >>= 1;
    }

    return c;
}

void square_matrix(uint8_t *Z) {
    uint8_t temp[16 * 16] = {0};

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                temp[i * 16 + j] ^= Mul[Z[i * 16 + k]][Z[k * 16 + j]];
            }
        }
    }
    memcpy(Z, temp, 16 * 16);
}

// Функция для заполнения матрицы умножения в поле Галуа
void fill_galois_multiplication_table(uint8_t table[FIELD_SIZE][FIELD_SIZE]) {
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            table[i][j] = GF_mult8(i, j);
        }
    }
}

void generate_LUT() {
    fill_galois_multiplication_table(Mul);
    uint8_t Z[256] = {
        0x94, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x20, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x85, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xC2, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xC2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
        0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
        0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint8_t Z_reverse[256] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94,
        0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85,
        0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC2,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFB,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xC2,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x85,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x94
    };
    square_matrix(Z);
    square_matrix(Z);
    square_matrix(Z);
    square_matrix(Z);
    square_matrix(Z_reverse);
    square_matrix(Z_reverse);
    square_matrix(Z_reverse);
    square_matrix(Z_reverse);
    for (int i = 0; i < 16; ++i)
        for (int b = 0; b < 256; ++b)
            for (int k = 0; k < 16; ++k) {
                LS_mat[i][b][k] = Mul[Pi[b]][Z[i * 16 + k]];
                LS_reverse_mat[i][b][k] = Mul[Pi_reverse[b]][Z_reverse[i * 16 + k]];
            }
}

// Функция X
void X(const chunk a, const chunk b, chunk c) {
    c[0] = a[0] ^ b[0];
    c[1] = a[1] ^ b[1];
}

void LS(uint8_t *in_out) {
    chunk result = {0};
    for (int i = 0; i < 16; ++i)
        X(result, (void *) LS_mat[i][in_out[i]], result);
    memcpy(in_out, result, sizeof(chunk));
}

void LS_reverse(uint8_t *in_out) {
    chunk result = {0};
    for (int i = 0; i < 16; ++i)
        X(result, (void *) LS_reverse_mat[i][in_out[i]], result);
    memcpy(in_out, result, sizeof(chunk));
}

// Функция S
void S(chunk in_out) {
    // Переход к представлению в байтах
    uint8_t *byte = (int8_t *) in_out;
    for (int i = 0; i < KUZNECHIK_BLOCK_SIZE; i++) {
        byte[i] = Pi[byte[i]];
    }
}

// Обратная функция S
void S_reverse(chunk in_out) {
    // Переход к представлению в байтах
    uint8_t *byte = (int8_t *) in_out;
    for (int i = 0; i < KUZNECHIK_BLOCK_SIZE; i++) {
        byte[i] = Pi_reverse[byte[i]];
    }
}


// Функция R
void R(uint8_t *in_out) {
    // Аккумулятор
    uint8_t acc = in_out[15];
    // Переход к представлению в байтах
    uint8_t *byte = (int8_t *) in_out;
    for (int i = 14; i >= 0; i--) {
        byte[i + 1] = byte[i];
        // acc ^= GF_mult8(byte[i], linear_vector[i]);
        acc ^= Mul[byte[i]][linear_vector[i]];
    }
    byte[0] = acc;
}

// Обратная функция R
void R_reverse(uint8_t *in_out) {
    // Аккумулятор
    uint8_t acc = in_out[0];
    // Переход к представлению в байтах
    uint8_t *byte = (int8_t *) in_out;

    for (int i = 0; i < 15; i++) {
        byte[i] = byte[i + 1];
        // acc ^= GF_mult8(byte[i], linear_vector[i]);
        acc ^= Mul[byte[i]][linear_vector[i]];
    }

    byte[15] = acc;
}

// Функция L
void L(uint8_t *in_out) {
    for (int i = 0; i < KUZNECHIK_BLOCK_SIZE; i++)
        R(in_out);
}

// Обратная функция L
void L_reverse(uint8_t *in_out) {
    // Счетчик
    for (int i = 0; i < 16; i++)
        R_reverse(in_out);
}

// Генерация итерационных ключей
void gen_round_keys(const uint8_t *key, chunk *round_keys) {
    // Счетчик
    int i;
    // Константы
    uint8_t cs[32][KUZNECHIK_BLOCK_SIZE] = {};

    // Генерация констант с помощью L-преобразования номера итерации
    for (i = 0; i < 32; i++) {
        cs[i][15] = i + 1;
        L(cs[i]);
    }

    // Итерационные ключи (четный и нечетный)
    chunk ks[2] = {};
    // Разместим ключ шифрования
    // результат = итерационный ключ = (преобразование к указателю на чанк)[номер чанка][часть чанка]
    round_keys[0][0] = ks[0][0] = ((chunk *) key)[0][0];
    round_keys[0][1] = ks[0][1] = ((chunk *) key)[0][1];
    round_keys[1][0] = ks[1][0] = ((chunk *) key)[1][0];
    round_keys[1][1] = ks[1][1] = ((chunk *) key)[1][1];

    // Генерация оставшихся ключей с использованием констант
    for (i = 1; i <= 32; i++) {
        // Новый ключ
        chunk new_key = {0};

        // Преобразование X
        // (void*) для избежания предупреждений о неверном типе, передаваемом в функцию
        X(ks[0], (void *) cs[i - 1], new_key);
        LS((void *) new_key);
        // // Преобразование S
        // S(new_key);
        // // Преобразование L
        // // (uint8_t*) для избежания предупреждений о неверном типе, передаваемом в функцию
        // L((uint8_t *) &new_key);
        // Преобразование X
        X(new_key, ks[1], new_key);

        // Сдвигаем ключи
        ks[1][0] = ks[0][0];
        ks[1][1] = ks[0][1];

        // Записываем новый ключ
        ks[0][0] = new_key[0];
        ks[0][1] = new_key[1];

        // Каждую 8 итерацию сети Фейстеля за исключением нулевой запишем ключи
        if ((i > 0) && (i % 8 == 0)) {
            round_keys[(i >> 2)][0] = ks[0][0];
            round_keys[(i >> 2)][1] = ks[0][1];

            round_keys[(i >> 2) + 1][0] = ks[1][0];
            round_keys[(i >> 2) + 1][1] = ks[1][1];
        }
    }
}

void aligned_load_array_to_L1(int *arr, size_t size) {
    for (size_t i = 0; i < size; i++)
        // Предполагается, что массив выровнен по границе 64 байта
        _mm_prefetch((const char*)&arr[i], _MM_HINT_T0);  // Подгрузка в L1
}


// Функция шифрования
// Поддерживает запись результата в исходный массив
void kuznechik_encrypt(chunk *round_keys, chunk in, chunk out) {
    // Буфер
    chunk p;
    // Создадим копию входных данных
    memcpy(p, in, sizeof(chunk));
    // В течении 10 итераций
    for (int i = 0; i < 9; i++) {
        X(p, round_keys[i], p);
        // LS((void *) p);
        LS((void *) p);
        // S(p);
        // // Преобразование L
        // L((uint8_t *) &p);
    }
    // Преобразование X
    X(p, round_keys[9], p);
    // Копируем полученный результат
    memcpy(out, p, sizeof(chunk));
}

void kuznechik_decrypt(chunk *round_keys, chunk in, chunk out) {
    // Буфер
    chunk p;
    // Создадим копию входных данных
    memcpy(p, in, sizeof(chunk));
    S(p);
    for (int i = 9; i > 0; --i) {
        LS_reverse((void *) p);
        X(p, round_keys[i], p);
    }
    S_reverse(p);
    X(p, round_keys[0], p);
    memcpy(out, p, sizeof(chunk));
}

// Печать чанка
void print_chunk(chunk p) {
    int i;
    for (i = 0; i < sizeof(chunk); i++)
        printf("0x%02X ", ((uint8_t *) p)[i]);

    printf("\n");
}

void print(uint8_t *p, int size) {
    for (int i = 0; i < size; i++)
        printf("0x%02X ", p[i]);

    printf("\n");
}

// Функция для записи массива в файл
void write_galois_multiplication_table(const char *filename, uint8_t table[FIELD_SIZE][FIELD_SIZE]) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Ошибка при открытии файла");
        return;
    }

    // Запись таблицы в файл
    fwrite(table, sizeof(uint8_t), FIELD_SIZE * FIELD_SIZE, fp);

    fclose(fp);
}


// int main() {
//     generate_LUT();
//     uint8_t key[] = {
//         0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
//         0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
//         0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
//         0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
//     };
//     chunk round_keys[10] = {};
//     gen_round_keys(key, round_keys);
//     uint8_t data[KUZNECHIK_BLOCK_SIZE] = {
//         0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00,
//         0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88
//     };
//     printf("Plain text:\n");
//     print_chunk((void *) data);
//     chunk encrypted;
//     memcpy(encrypted, data, sizeof(chunk));
//     int enc_dec_times = 100000;
//     clock_t start_time = clock();
//     for (int i = 0; i < enc_dec_times; i++) {
//         kuznechik_encrypt(round_keys, (void *) encrypted, encrypted);
//     }
//     clock_t end_time = clock();
//     double elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
//     printf("Encoded text:\n");
//     print_chunk(encrypted);
//     printf("Encryption speed: %.6f MB/sec\n", (enc_dec_times * KUZNECHIK_BLOCK_SIZE) / elapsed_time / 1024 / 1024);
//     start_time = clock();
//     for (int i = 0; i < enc_dec_times; i++) {
//         kuznechik_decrypt(round_keys, (void *) encrypted, encrypted);
//     }
//     end_time = clock();
//     elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
//     printf("Decoded text:\n");
//     print_chunk(encrypted);
//     printf("Decryption speed: %.6f MB/sec\n", (enc_dec_times * KUZNECHIK_BLOCK_SIZE) / elapsed_time / 1024 / 1024);
//     return 0;
// }



void test_asm() {
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
    printf("Encryption KUZNECHICK_C speed: %.6f MB/sec\n", (enc_dec_times*KUZNECHIK_BLOCK_SIZE)/elapsed_time/1024/1024);
    memcpy(encrypted, data, sizeof(chunk));
    start_time = clock();
    for (int i = 0; i < enc_dec_times; i++) {
        kyznechick_asm_133(round_keys, (void *) encrypted, encrypted);
    }
    end_time = clock();
    elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Encoded text:\n");
    print_chunk(encrypted);
    printf("Encryption KUZNECHICK_ASM133 speed: %.6f MB/sec\n",  (enc_dec_times*KUZNECHIK_BLOCK_SIZE)/elapsed_time/1024/1024);
    memcpy(encrypted, data, sizeof(chunk));
    start_time = clock();
    for (int i = 0; i < enc_dec_times; i++) {
        kyznechick_asm_158(round_keys, (void *) encrypted, encrypted);
    }
    end_time = clock();
    elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Encoded text:\n");
    print_chunk(encrypted);
    printf("Encryption KUZNECHICK_ASM_152 speed: %.6f MB/sec\n",  (enc_dec_times*KUZNECHIK_BLOCK_SIZE)/elapsed_time/1024/1024);

}

