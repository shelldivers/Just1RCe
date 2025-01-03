# ONLY UNIX & MAC!!
#
# Just1RCe - CMake Configuration
#
# Features:
# - Builds the Just1RCe project (main executable)
# - Python virtual environment is used for code quality tools
#   - cppcheck: Static code analysis with HTML and XML reports
#   - cpplint: Style checking with report generation
# - Includes configurable presets for CI/CD or local development
#
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
#
# 4. Run google test only:
#    cmake --build build --target RunGoogleTest

# CMake required minimum version
cmake_minimum_required(VERSION 3.30)

# Project name & configuration
project(Just1RCe VERSION 1.0.0)

# Set policy CMP0135 to NEW for not set DOWNLOAD_EXTRACT_TIMESTAMP
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# Add executable files
add_executable(Just1RCe
    #executable files
)

set(CMAKE_SH "bash")

# Set project configuration options
set(AUTHOR_NAME "SHELLDIVERS")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure reports directory exists
set(REPORTS_DIR "${CMAKE_SOURCE_DIR}/reports")

file(MAKE_DIRECTORY ${REPORTS_DIR})

# Configure file for generating config.h
configure_file(config.h.in ${CMAKE_SOURCE_DIR}/includes/config.h)

# Include directories
target_include_directories(
    Just1RCe PUBLIC
    ${CMAKE_BINARY_DIR}/includes
    ${CMAKE_SOURCE_DIR}/includes
)

#=============== Set Python Virtual Environment ===============
# Find Python Interpreter
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Python Virtual Environment Paths
set(VENV_PATH "${CMAKE_BINARY_DIR}/venv")
set(VENV_PIP "${VENV_PATH}/bin/pip")

# Scripts path
set(SCRIPTS_PATH "${CMAKE_SOURCE_DIR}/scripts")

# Modify report output paths
set(FILE_LIST "${CMAKE_BINARY_DIR}/file_list.txt")
set(CPPLINT_REPORT "${REPORTS_DIR}/cpplint_report.txt")
set(CPPCHECK_REPORT_XML "${REPORTS_DIR}/report_cppcheck.xml")
set(CPPCHECK_HTML_DIR "${REPORTS_DIR}/cppcheck_html")

# Create a Python Virtual Environment
add_custom_target(
    CreateVenv 
    COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_PATH}
    COMMENT "Creating Python virtual environment at ${VENV_PATH}"
    VERBATIM
)

add_custom_target(
    GenerateFileList 
    COMMAND find ${CMAKE_SOURCE_DIR}/srcs -type f -name "*.cc" > ${FILE_LIST}
    COMMAND find ${CMAKE_SOURCE_DIR}/includes -type f -name "*.h" >> ${FILE_LIST}
    COMMENT "Generating file list"
)

#=============== Set google test ===============
# Set google test path
set(TEST_DIR "${CMAKE_SOURCE_DIR}/tests")

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG f8d7d77 # v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    googleUnitTest
    # test files
)

# Link GoogleTest
target_link_libraries(
    googleUnitTest
    # must write main function at test file
    GTest::gtest 
    GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(
    googleUnitTest   
)

add_custom_target(
    RunGoogleTest
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMENT "Running GoogleTest with CTest"
)

#=============== Set cppcheck ===============
# Set cppcheck path
set(CPPCHECK "${VENV_PATH}/bin/cppcheck")
set(CPPCHECK_HTMLREPORT "${CMAKE_SOURCE_DIR}/scripts/cppcheck-htmlreport")

# Step 1: Install cppcheck and Pygments in the Virtual Environment
add_custom_target(
    InstallCppcheckAndPygments 
    COMMAND ${VENV_PIP} install cppcheck
    COMMAND ${VENV_PIP} install Pygments
    DEPENDS CreateVenv
    COMMENT "Installing Pygments in the Python virtual environment"
)

# Step 2: Run cppcheck
add_custom_target(
    RunCppcheckXmlReport
    COMMAND xargs ${CPPCHECK} --language=c++ --enable=all --suppress=missingIncludeSystem --xml --output-file=${CPPCHECK_REPORT_XML} < ${FILE_LIST}
    DEPENDS GenerateFileList InstallCppcheckAndPygments
    COMMENT "Running cppcheck and generating XML report"
)

# Step 3: Add target to run cppcheck-htmlreport
add_custom_target(
    RunCppcheck
    COMMAND ${VENV_PATH}/bin/python ${CPPCHECK_HTMLREPORT} --file=${CPPCHECK_REPORT_XML} --report-dir=${CPPCHECK_HTML_DIR}
    DEPENDS RunCppcheckXmlReport
    COMMENT "Generating cppcheck HTML report using virtual environment"
)

#=============== Set cpplint ===============
# Set cpplint path
set(CPPLINT "${VENV_PATH}/bin/cpplint")
set(CPPLINT_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/cpplint.sh")

# Step 1: Install cpplint in the Virtual Environment
add_custom_target(
    InstallCpplint
    COMMAND ${VENV_PIP} install cpplint
    DEPENDS CreateVenv
    COMMENT "Installing cpplint in the Python virtual environment"
)

# Step 2: Run cpplint
add_custom_target(
    RunCpplint
    COMMAND ${CPPLINT_SCRIPT} ${CPPLINT_REPORT} ${CPPLINT} ${FILE_LIST}
    DEPENDS InstallCpplint GenerateFileList
    COMMENT "Running cpplint and saving results to cpplint_report.txt"
)

