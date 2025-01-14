cmake_minimum_required(VERSION 3.30)
project(C_Kuznechik_GOST_R_34_12_2015 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -march=native")
add_executable(C_Kuznechik_GOST_R_34_12_2015
        MGM.c)
