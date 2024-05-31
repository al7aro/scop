cmake_minimum_required(VERSION 3.25.0)
set(PROJECT_NAME scop)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR/bin})

set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(VENDOR_DIR ${PROJECT_DIR}/vendor)
set(LIBS_DIR ${PROJECT_DIR}/libs)

# scop_math
set(SCOP_ENGINE_DIR ${LIBS_DIR}/scop_engine)
add_subdirectory(${SCOP_ENGINE_DIR})

# Project Sources
file(GLOB_RECURSE source_files
        "${PROJECT_DIR}/src/scop_main.c"
        "${PROJECT_DIR}/include/scop_main.h"
        "${PROJECT_DIR}/src/input.c"
)

foreach (src ${source_files})
    get_filename_component(dir ${src} DIRECTORY)
    string(REGEX MATCH "scop/(.*)" CMAKE_MATCH_1 ${dir})
    string(REPLACE "scop/" "" desired_portion ${CMAKE_MATCH_1})
    string(REGEX REPLACE "^(scop\/)(.*)" "\\2" desired_portion ${CMAKE_MATCH_1})
    message(${src})
    source_group(${desired_portion} FILES ${src})
endforeach()

set(SOURCES ${source_files})

# Assets
file(GLOB_RECURSE shader_files
    "${PROJECT_DIR}/assets/shaders/*.vert"
    "${PROJECT_DIR}/assets/shaders/*.frag"
)
source_group("Assets\\Shaders" FILES ${shader_files})

file(GLOB_RECURSE texture_files
    "${PROJECT_DIR}/assets/textures/*.pam"
)
source_group("Assets\\Textures" FILES ${texture_files})

file(GLOB_RECURSE model_files
    "${PROJECT_DIR}/assets/models/*.obj"
    "${PROJECT_DIR}/assets/models/*.mtl"
)
source_group("Assets\\Models" FILES ${model_files})

# Executable
add_executable(${PROJECT_NAME} ${SOURCES} ${shader_files})

add_compile_definitions(_CRT_SECURE_NO_DEPRECATE)

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

# Set the output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Libraries
target_link_libraries(${PROJECT_NAME} scop_engine)

# Headers
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/libs/scop_engine/include")


install(TARGETS ${PROJECT_NAME})
