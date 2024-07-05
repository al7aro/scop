cmake_minimum_required(VERSION 3.25.0)
set(PROJECT_NAME scop)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR/bin})

set(PROJECT_DIR .)
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

# OpenGL
find_package(OpenGL REQUIRED)

# Project Sources
file(GLOB_RECURSE ALL_SOURCES
        "${PROJECT_DIR}/src/**/*.c"
        "${PROJECT_DIR}/include/**/*.h"
)

foreach (src ${ALL_SOURCES})
    get_filename_component(dir ${src} DIRECTORY)
	message(${src})
    string(REGEX MATCH "scop/.(.*)" CMAKE_MATCH_1 ${dir})
    string(REPLACE "scop/." "" desired_portion ${CMAKE_MATCH_1})
    string(REGEX REPLACE "^(scop\/.)(.*)" "\\2" desired_portion ${CMAKE_MATCH_1})
    source_group(${desired_portion} FILES ${src})
endforeach()

# Assets
file(GLOB_RECURSE shader_files
    "${PROJECT_DIR}/assets/shaders/*"
    "${PROJECT_DIR}/assets/shaders/*"
)
source_group("Assets\\Shaders" FILES ${shader_files})

# Executable
add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${shader_files})

add_compile_definitions(_CRT_SECURE_NO_DEPRECATE)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_options(${PROJECT_NAME} PRIVATE 
        -Wall
        -Wextra
        -Werror
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
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# Headers
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/scop_setup")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/scop_engine")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/scop_engine/shader")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/scop_engine/scop_obj_loader")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/scop_engine/scop_math")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/scop_engine/scop_image")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/scop_engine/libft_ll")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/scop_engine/core")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/scop_engine/core/basic_geometry")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DIR}/include/scop_engine/core/renderer")

target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLAD_DIR}/include")

# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

message("Scop install dir: [" ${PROJECT_SOURCE_DIR} "]")

add_compile_definitions(SCOP_INSTALL_PATH=\"${PROJECT_SOURCE_DIR}\")

install(TARGETS ${PROJECT_NAME})
