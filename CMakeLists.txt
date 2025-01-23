cmake_minimum_required(VERSION 3.30)
project(C_Kuznechik_GOST_R_34_12_2015 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -march=native -mavx2 -mfma")

enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_FLAGS "-f win64 -g")    # Флаг для NASM
set(SOURCES
        MGM.c
        Kuznechik.c
        funcs.asm
)

add_executable(C_Kuznechik_GOST_R_34_12_2015 ${SOURCES})
