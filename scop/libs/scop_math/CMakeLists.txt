cmake_minimum_required(VERSION 3.25.0)
project(scop_math)

add_library(scop_math scop_math.h scop_math.c)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_options(${PROJECT_NAME} PRIVATE 
        -Wall
        -Wextra
        -Werror 
        -pedantic
    )
endif()
if(MSVC) 
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
endif()

target_include_directories(scop_math PUBLIC ./)