cmake_minimum_required(VERSION 3.25.0)
project(px_image)

add_library(px_image px_image.h px_image.c px_header_parser.c px_data_reader.c)

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

target_include_directories(px_image PUBLIC ./)