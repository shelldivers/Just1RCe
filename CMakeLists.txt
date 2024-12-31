# ONLY UNIX & MAC!!
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
project(Just1RCe VERSION 1.0.0)

# Add executable files
add_executable(Just1RCe
  main.cc 
  srcs/math.cc 
  includes/math.h
)

set(CMAKE_SH "bash")

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

#=============== Set external programs =============== 
# Find Python Interpreter
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Python Virtual Environment Path
set(VENV_PATH "${CMAKE_BINARY_DIR}/venv")

# Ensure reports directory exists
set(REPORTS_DIR "${CMAKE_SOURCE_DIR}/reports")
file(MAKE_DIRECTORY ${REPORTS_DIR})

# Modify report output paths
set(CPPLINT_REPORT "${REPORTS_DIR}/cpplint_report.txt")
set(CPPCHECK_REPORT_XML "${REPORTS_DIR}/report_cppcheck.xml")
set(CPPCHECK_HTML_DIR "${REPORTS_DIR}/cppcheck_html")

# Create a Python Virtual Environment
add_custom_target(
    CreateVenv ALL
    COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_PATH}
    COMMENT "Creating Python virtual environment at ${VENV_PATH}"
    VERBATIM
)

add_custom_target(
    GenerateFileList ALL
    COMMAND find ../srcs -type f -name "*.cc" > ${CMAKE_BINARY_DIR}/file_list.txt
    COMMAND find ../includes -type f -name "*.h" >> ${CMAKE_BINARY_DIR}/file_list.txt
    COMMENT "Generating file list"
)

#=============== Set cppcheck ===============

# Step 1: Install cppcheck and Pygments in the Virtual Environment
add_custom_target(
    InstallCppcheckAndPygments 
    COMMAND ${VENV_PATH}/bin/pip install cppcheck
    COMMAND ${VENV_PATH}/bin/pip install Pygments
    DEPENDS CreateVenv
    COMMENT "Installing Pygments in the Python virtual environment"
)

# Step 2: Run cppcheck
add_custom_target(
    RunCppcheckXmlReport
    COMMAND xargs ${VENV_PATH}/bin/cppcheck --language=c++ --enable=all --suppress=missingIncludeSystem --xml --output-file=${CPPCHECK_REPORT_XML} < ${CMAKE_BINARY_DIR}/file_list.txt
    DEPENDS GenerateFileList InstallCppcheckAndPygments
    COMMENT "Running cppcheck and generating XML report"
)

# Step 3: Add target to run cppcheck-htmlreport
add_custom_target(
    RunCppcheck ALL
    COMMAND ${VENV_PATH}/bin/python ${CMAKE_SOURCE_DIR}/scripts/cppcheck-htmlreport --file=${CPPCHECK_REPORT_XML} --report-dir=${CPPCHECK_HTML_DIR}
    DEPENDS RunCppcheckXmlReport
    COMMENT "Generating cppcheck HTML report using virtual environment"
)

#|=============== End cppcheck ===============|

#=============== Set cpplint ===============

# Step 1: Install cpplint in the Virtual Environment
add_custom_target(
    InstallCpplint
    COMMAND ${VENV_PATH}/bin/pip install cpplint
    DEPENDS CreateVenv
    COMMENT "Installing cpplint in the Python virtual environment"
)

# Step 2: Run cpplint
add_custom_target(
    RunCpplint ALL
    COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/scripts/cpplint.sh ${CPPLINT_REPORT} ${VENV_PATH}/bin/cpplint ${CMAKE_BINARY_DIR}/file_list.txt
    DEPENDS InstallCpplint GenerateFileList
    COMMENT "Running cpplint and saving results to cpplint_report.txt"
)

#|=============== Set cpplint ===============|

# include(ExternalProject)

# #=============== Set Doxygen =============== 
# include(ExternalProject)

# # Define the installation directory for Doxygen
# set(DOXYGEN_INSTALL_DIR ${CMAKE_BINARY_DIR}/doxygen)
# # Set the path to the Doxyfile template
# set(DOXYFILE_TEMPLATE ${CMAKE_SOURCE_DIR}/Doxyfile.in)
# # Set the output path for the configured Doxyfile
# set(DOXYFILE_OUTPUT ${CMAKE_BINARY_DIR}/Doxyfile)
# # Set the path to the Doxygen executable
# set(DOXYGEN_EXECUTABLE ${DOXYGEN_INSTALL_DIR}/bin/doxygen)

# # Add an ExternalProject to download and build Doxygen
# ExternalProject_Add(
#     DoxygenProject
#     URL https://github.com/doxygen/doxygen/archive/refs/tags/Release_1_9_7.zip
#     PREFIX ${CMAKE_BINARY_DIR}/doxygen
#     SOURCE_DIR ${CMAKE_BINARY_DIR}/doxygen/src/DoxygenProject
#     BINARY_DIR ${CMAKE_BINARY_DIR}/doxygen/build
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DOXYGEN_INSTALL_DIR}
#     CONFIGURE_COMMAND cmake -S ${CMAKE_BINARY_DIR}/doxygen/src/DoxygenProject -B ${CMAKE_BINARY_DIR}/doxygen/build -DCMAKE_INSTALL_PREFIX=${DOXYGEN_INSTALL_DIR}
#     BUILD_COMMAND cmake --build ${CMAKE_BINARY_DIR}/doxygen/build
#     INSTALL_COMMAND cmake --install ${CMAKE_BINARY_DIR}/doxygen/build
# )

# # Configure the Doxyfile
# configure_file(${DOXYFILE_TEMPLATE} ${DOXYFILE_OUTPUT} @ONLY)

# # Add a custom target to generate documentation
# add_custom_target(
#     GenerateDocs
#     COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUTPUT}
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     COMMENT "Generating Doxygen documentation"
# )

# # Add dependency to ensure Doxygen is built before documentation is generated
# add_dependencies(GenerateDocs DoxygenProject)

#|=============== End Doxygen Configuration ===============|

#|=============== End set external programs ===============|


# Just1RCe - CMake Configuration
#
# Features:
# - Builds the Just1RCe project (main executable)
# - Python virtual environment is used for code quality tools
#   - cppcheck: Static code analysis with HTML and XML reports
#   - cpplint: Style checking with report generation
# - Includes configurable presets for CI/CD or local development
#
# To build and run tools:
# 1. Default build (includes all targets): 
#    cmake --preset=default -S . -B build
#    cmake --build build
#
# 2. Run cpplint only:
#    cmake --build build --target RunCpplint
#
# 3. Run cppcheck only:
#    cmake --build build --target RunCppcheck


