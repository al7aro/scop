cmake_minimum_required(VERSION 3.25.0)
project(scop_math)

file(GLOB_RECURSE source_files
    math_ND.c
    math_234D.c
    scop_math.h
)
set(SOURCES ${source_files})

add_library(scop_math ${SOURCES})

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