cmake_minimum_required(VERSION 3.25)
project(code C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(.)

# Collect all C source files (excluding test files and main.c)
file(GLOB LIB_SOURCES "*.c")
list(FILTER LIB_SOURCES EXCLUDE REGEX "test_.*\\.c$")
list(FILTER LIB_SOURCES EXCLUDE REGEX "main\\.c$")

# Main executable
add_executable(mastermind_game main.c ${LIB_SOURCES})

# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

# Test executable
add_executable(test_main test_main.cpp ${LIB_SOURCES})
target_link_libraries(test_main PRIVATE Catch2::Catch2WithMain)

catch_discover_tests(test_main)
