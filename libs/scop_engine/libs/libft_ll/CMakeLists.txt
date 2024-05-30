cmake_minimum_required(VERSION 3.25.0)
project(libft_ll)

add_library(libft_ll include/libft_ll.h src/libft_ll.c)
target_include_directories(libft_ll PUBLIC include/)
