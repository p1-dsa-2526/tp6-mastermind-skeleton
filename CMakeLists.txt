cmake_minimum_required(VERSION 3.25)
project(code C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

# Main executable
add_executable(mastermind_game
    display.c
    display.h
    main.c
    mastermind.c
    mastermind.h)

# Test executable for mastermind functions
add_executable(test_mastermind
    test_mastermind.c
    mastermind.c
    mastermind.h)

# Test executable for display functions
add_executable(test_display
    test_display.c
    display.c
    display.h
    mastermind.c
    mastermind.h)

# Enable CTest
enable_testing()

# Mastermind tests
set(MASTERMIND_TEST_SUITES random_comb well_placed present check_result)
foreach(suite ${MASTERMIND_TEST_SUITES})
    add_test(
        NAME MastermindTests.${suite}
        COMMAND test_mastermind --filter=${suite}.* --output=${CMAKE_SOURCE_DIR}/test_reports/test_mastermind.${suite}.xml
    )
    set_tests_properties(MastermindTests.${suite} PROPERTIES
        TIMEOUT 30
        LABELS "unit"
    )
endforeach()

# Display tests
set(DISPLAY_TEST_SUITES init_grid free_grid update_grid integration)
foreach(suite ${DISPLAY_TEST_SUITES})
    add_test(
        NAME DisplayTests.${suite}
        COMMAND test_display --filter=${suite}.* --output=${CMAKE_SOURCE_DIR}/test_reports/test_display.${suite}.xml
    )
    set_tests_properties(DisplayTests.${suite} PROPERTIES
        TIMEOUT 30
        LABELS "unit"
    )
endforeach()
