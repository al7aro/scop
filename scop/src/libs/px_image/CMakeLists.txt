cmake_minimum_required(VERSION 3.25.0)
project(px_image)

add_library(px_image px_image.h px_image.c)
target_include_directories(px_image PUBLIC ./)