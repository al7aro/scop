cmake_minimum_required(VERSION 3.25.0)
project(px_image)

add_library(px_image px_image.h px_image.c px_header_parser.c px_data_reader.c)
target_include_directories(px_image PUBLIC ./)