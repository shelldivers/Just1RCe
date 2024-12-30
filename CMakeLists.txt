# ONLY MAC!!
#
#
#
#

# CMake required minimum version
cmake_minimum_required(VERSION 3.30)

# Set policy CMP0135 to NEW for not set DOWNLOAD_EXTRACT_TIMESTAMP
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# Project name & configuration
project(Just1RCe VERSION 1.0.1)

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
configure_file(config.h.in ${CMAKE_SOURCE_DIR}/includes/config.h)

# Set C++ standard virsion
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
target_include_directories(Just1RCe PUBLIC
${CMAKE_BINARY_DIR}/includes
${CMAKE_SOURCE_DIR}/includes
)

#=============== Set cppcheck ===============

# Python Virtual Environment Path
set(VENV_PATH "${CMAKE_BINARY_DIR}/venv")

# Find Python Interpreter
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Step 1: Create a Python Virtual Environment
add_custom_target(
    CreateVenv ALL
    COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_PATH}
    COMMENT "Creating Python virtual environment at ${VENV_PATH}"
)

# Step 2: Install cppcheck in the Virtual Environment
add_custom_target(
    InstallCppcheck ALL
    COMMAND ${VENV_PATH}/bin/pip install cppcheck
    DEPENDS CreateVenv
    COMMENT "Installing cppcheck in the Python virtual environment"
)

# Step 3: Generate a File List for cppcheck
add_custom_target(
    GenerateFileList
    COMMAND find . \\( -name "*.cc" -path "./srcs/**" \\) -o \\( -name "*.h" -path "./includes/**" \\) > ${CMAKE_BINARY_DIR}/file_list.txt
    COMMENT "Generating file list for cppcheck"
)

# Step 4: Run cppcheck using the Virtual Environment
add_custom_target(
    RunCppcheck
    COMMAND ${VENV_PATH}/bin/cppcheck --language=c++ --enable=all --suppress=missingIncludeSystem --xml --output-file=${CMAKE_BINARY_DIR}/report.xml @${CMAKE_BINARY_DIR}/file_list.txt
    DEPENDS InstallCppcheck GenerateFileList
    COMMENT "Running cppcheck for static code analysis"
)

# Step 5: Clean up the file list (Optional)
add_custom_target(
    CleanupFileList
    COMMAND rm -f ${CMAKE_BINARY_DIR}/file_list.txt
    DEPENDS RunCppcheck
    COMMENT "Cleaning up file list"
)

#=============== End cppcheck ===============


#=============== Set external programs =============== 
include(ExternalProject)

#=============== Set Doxygen =============== 
include(ExternalProject)

# Define the installation directory for Doxygen
set(DOXYGEN_INSTALL_DIR ${CMAKE_BINARY_DIR}/doxygen)
# Set the path to the Doxyfile template
set(DOXYFILE_TEMPLATE ${CMAKE_SOURCE_DIR}/Doxyfile.in)
# Set the output path for the configured Doxyfile
set(DOXYFILE_OUTPUT ${CMAKE_BINARY_DIR}/Doxyfile)
# Set the path to the Doxygen executable
set(DOXYGEN_EXECUTABLE ${DOXYGEN_INSTALL_DIR}/bin/doxygen)

# Add an ExternalProject to download and build Doxygen
ExternalProject_Add(
    DoxygenProject
    URL https://github.com/doxygen/doxygen/archive/refs/tags/Release_1_9_7.zip
    PREFIX ${CMAKE_BINARY_DIR}/doxygen
    SOURCE_DIR ${CMAKE_BINARY_DIR}/doxygen/src/DoxygenProject
    BINARY_DIR ${CMAKE_BINARY_DIR}/doxygen/build
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DOXYGEN_INSTALL_DIR}
    CONFIGURE_COMMAND cmake -S ${CMAKE_BINARY_DIR}/doxygen/src/DoxygenProject -B ${CMAKE_BINARY_DIR}/doxygen/build -DCMAKE_INSTALL_PREFIX=${DOXYGEN_INSTALL_DIR}
    BUILD_COMMAND cmake --build ${CMAKE_BINARY_DIR}/doxygen/build
    INSTALL_COMMAND cmake --install ${CMAKE_BINARY_DIR}/doxygen/build
)

# Configure the Doxyfile
configure_file(${DOXYFILE_TEMPLATE} ${DOXYFILE_OUTPUT} @ONLY)

# Add a custom target to generate documentation
add_custom_target(
    GenerateDocs
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUTPUT}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating Doxygen documentation"
)

# Add dependency to ensure Doxygen is built before documentation is generated
add_dependencies(GenerateDocs DoxygenProject)

#=============== End Doxygen Configuration ===============

#=============== End set external programs =============== 

# cmake --preset=default -S . -B build


# cmake --build build --target InstallCppcheck
# cmake --build build --target RunCppcheck

# cmake --build build --target DoxygenProject
# cmake --build build --target GenerateDocs

# cmake --build build --target CleanupFileList




