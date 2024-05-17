cmake_minimum_required(VERSION 3.26)
project(3ind C)

set(CMAKE_C_STANDARD 23)

add_executable(3ind main.c
        sysfun.h
        sysfun.c
)
