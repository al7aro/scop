cmake_minimum_required(VERSION 3.25.0)
set(PROJECT_NAME scop)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR/bin})

set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})
set(VENDOR_DIR ${PROJECT_DIR}/vendor)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_DIR ${VENDOR_DIR}/glfw)
add_subdirectory(${GLFW_DIR})

# GLAD
set(GLAD_DIR ${VENDOR_DIR}/glad)
add_subdirectory(${GLAD_DIR})

# OpenGL
find_package(OpenGL REQUIRED)

# Project Sources
file(GLOB_RECURSE source_files
    "${PROJECT_DIR}/src/*.c"
    "${PROJECT_DIR}/src/include/*.h"
)

file(GLOB_RECURSE shader_files
    "${PROJECT_DIR}/src/shaders/*.vert"
    "${PROJECT_DIR}/src/shaders/*.frag"
)
source_group("Shaders" FILES ${shader_files})

set(SOURCES ${source_files})

# Executable
add_executable(${PROJECT_NAME} ${SOURCES} ${shader_files})

# Set the output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Libraries
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# Headers
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/src/include")

target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLAD_DIR}/include")

# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

install(TARGETS ${PROJECT_NAME})
