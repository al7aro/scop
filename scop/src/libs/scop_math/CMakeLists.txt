cmake_minimum_required(VERSION 3.25.0)
project(scop_math)

add_library(scop_math scop_math.h scop_math.c)
target_include_directories(scop_math PUBLIC ./)