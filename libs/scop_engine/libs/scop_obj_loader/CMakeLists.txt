cmake_minimum_required(VERSION 3.25.0)
project(scop_obj_loader)

add_library(scop_obj_loader scop_obj_loader.h scop_obj_loader.c scop_mtl_loader.c scop_obj_loader_utils.c)

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

set(LIBS_DIR ../)
set(LIBFT_LL_DIR ${LIBS_DIR}/libft_ll)

target_include_directories(scop_obj_loader PUBLIC ./)
target_include_directories(scop_obj_loader PUBLIC ${LIBFT_LL_DIR}/include)