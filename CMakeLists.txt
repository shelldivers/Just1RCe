# ONLY MAC!!
#
#
#
#

# CMake required minimum version
cmake_minimum_required(VERSION 3.10)

# Project name & configuration
project(Just1RCe VERSION 1.0.0)

# Add executable files
add_executable(Just1RCe
  main.cc 
  srcs/math.cc 
  includes/math.h
)

# Set project configuration options
set(ENABLE_FEATURE_X ON)
set(AUTHOR_NAME "jiwojung")


# Configure file for generating config.h
configure_file(config.h.in ${CMAKE_BINARY_DIR}/includes/config.h)

# Set C++ standard virsion
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
target_include_directories(Just1RCe PUBLIC
${CMAKE_BINARY_DIR}/includes
${CMAKE_SOURCE_DIR}/includes
)

# Set graphviz option
include(CMakeGraphVizOptions)

set(CMAKE_GRAPHVIZ_GENERATE ON)
set(CMAKE_GRAPHVIZ_OUTPUT_FILE ${CMAKE_BINARY_DIR})
set(CMAKE_GRAPHVIZ_ENABLE_LEGEND OFF)
set(CMAKE_GRAPHVIZ_MAX_LEVEL 2)

add_custom_target(source_files ALL DEPENDS
    ${CMAKE_SOURCE_DIR}/main.cc
    ${CMAKE_SOURCE_DIR}/srcs/math.cc
    ${CMAKE_SOURCE_DIR}/includes/math.h
)

add_dependencies(Just1RCe source_files)

# cmake --preset=default -S .
# cmake --build build


# cmake --preset=default --graphviz=Just1RCe.dot -S .
# dot -Tpdf input.dot -o output.pdf




