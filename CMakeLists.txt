cmake_minimum_required(VERSION 3.28)
project(TeaShop C)

set(CMAKE_C_STANDARD 23)

add_executable(TeaShop main.c
        functions.h
        functions.c)
