cmake_minimum_required(VERSION 3.25)
project(code C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

# Collect all source files (excluding test files and main.c)
file(GLOB LIB_SOURCES "*.c")
list(FILTER LIB_SOURCES EXCLUDE REGEX "test_.*\\.c$")
list(FILTER LIB_SOURCES EXCLUDE REGEX "main\\.c$")

# Main executable
add_executable(mastermind_game main.c ${LIB_SOURCES})

# Auto-discover test files and create test executables
enable_testing()
file(GLOB TEST_FILES "test_*.c")

foreach(TEST_FILE ${TEST_FILES})
    # Get executable name from filename (e.g., test_mastermind.c -> test_mastermind)
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # Create test executable
    add_executable(${TEST_NAME} ${TEST_FILE} ${LIB_SOURCES})

    # Discover UTEST() macros in the test file
    file(READ ${TEST_FILE} TEST_SOURCE)
    string(REGEX MATCHALL "UTEST\\([^,]+,[^)]+\\)" TEST_MATCHES "${TEST_SOURCE}")

    foreach(match ${TEST_MATCHES})
        string(REGEX REPLACE "UTEST\\([ ]*([^, ]+)[ ]*,[ ]*([^) ]+)[ ]*\\)" "\\1.\\2" TEST_CASE "${match}")
        add_test(
            NAME ${TEST_NAME}.${TEST_CASE}
            COMMAND ${TEST_NAME} --filter=${TEST_CASE} --output=${CMAKE_SOURCE_DIR}/test_reports/${TEST_NAME}.${TEST_CASE}.xml
        )
        set_tests_properties(${TEST_NAME}.${TEST_CASE} PROPERTIES TIMEOUT 30 LABELS "unit")
    endforeach()
endforeach()
