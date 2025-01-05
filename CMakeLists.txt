cmake_minimum_required(VERSION 3.27)
project(GOST_R_34_12_2015)

set(CMAKE_CXX_STANDARD 14)

include_directories(kuznyechik)

add_executable(GOST_R_34_12_2015
        kuznyechik/gost12_15.cpp
        kuznyechik/gost12_15.h
        kuznyechik/main.cpp)
