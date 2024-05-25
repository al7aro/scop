cmake_minimum_required(VERSION 3.25.0)
set(PROJECT_NAME scop)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR/bin})

set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})
set(VENDOR_DIR ${PROJECT_DIR}/vendor)
set(LIBS_DIR ${PROJECT_DIR}/libs)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_DIR ${VENDOR_DIR}/glfw)
add_subdirectory(${GLFW_DIR})

# GLAD
set(GLAD_DIR ${VENDOR_DIR}/glad)
add_subdirectory(${GLAD_DIR})

# px_image
set(PX_IMAGE_DIR ${LIBS_DIR}/px_image)
add_subdirectory(${PX_IMAGE_DIR})

# scop_model_loader
set(SCOP_MODEL_LOADER_DIR ${LIBS_DIR}/scop_model_loader)
add_subdirectory(${SCOP_MODEL_LOADER_DIR})

# scop_math
set(SCOP_MATH_DIR ${LIBS_DIR}/scop_math)
add_subdirectory(${SCOP_MATH_DIR})

# libft_ll
set(LIBFT_LL_DIR ${LIBS_DIR}/libft_ll)
add_subdirectory(${LIBFT_LL_DIR})

# OpenGL
find_package(OpenGL REQUIRED)

# Project Sources
file(GLOB_RECURSE source_files
        "${PROJECT_DIR}/src/core/main.c"
        "${PROJECT_DIR}/include/core/main.h"
        "${PROJECT_DIR}/src/core/basic_mesh.c"
        "${PROJECT_DIR}/include/core/basic_mesh.h"
		"${PROJECT_DIR}/src/core/basic_model.c"
        "${PROJECT_DIR}/include/core/basic_model.h"

        "${PROJECT_DIR}/src/shader/shader.c"
        "${PROJECT_DIR}/include/shader/shader.h"
)

foreach (src ${source_files})
    get_filename_component(dir ${src} DIRECTORY)
    string(REGEX MATCH "scop/scop/(.*)" CMAKE_MATCH_1 ${dir})
    string(REPLACE "scop/scop/" "" desired_portion ${CMAKE_MATCH_1})
    string(REGEX REPLACE "^(scop\/scop\/)(.*)" "\\2" desired_portion ${CMAKE_MATCH_1})
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
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} px_image)
target_link_libraries(${PROJECT_NAME} scop_model_loader)
target_link_libraries(${PROJECT_NAME} scop_math)
target_link_libraries(${PROJECT_NAME} libft_ll)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# Headers
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/core")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/math")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/shader")

target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLAD_DIR}/include")

# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

install(TARGETS ${PROJECT_NAME})
