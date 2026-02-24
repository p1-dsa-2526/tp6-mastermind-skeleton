#include "utest.h"
#include "display.h"
#include <string.h>

#define TEST_MAX_ATTEMPTS 8

// Test init_grid() function
UTEST(init_grid, allocates_memory) {
    char * grid[TEST_MAX_ATTEMPTS];

    init_grid(grid);

    // Check that all grid rows are allocated
    for (int i = 0; i < TEST_MAX_ATTEMPTS; i++) {
        ASSERT_TRUE(grid[i] != NULL);
    }

    free_grid(grid);
}

UTEST(init_grid, initializes_with_correct_format) {
    char * grid[TEST_MAX_ATTEMPTS];

    init_grid(grid);

    // Check the initial format "- - - - | - - - -"
    const char expected[] = "- - - - | - - - -";
    for (int i = 0; i < TEST_MAX_ATTEMPTS; i++) {
        ASSERT_STREQ(expected, grid[i]);
    }

    free_grid(grid);
}

// Test free_grid() function
UTEST(free_grid, deallocates_memory) {
    char * grid[TEST_MAX_ATTEMPTS];

    init_grid(grid);

    // This test mainly ensures no segfault occurs
    // Memory leak detection would require valgrind or similar tools
    free_grid(grid);

    ASSERT_TRUE(1); // If we get here without crash, test passes
}

// Test update_grid() function
UTEST(update_grid, first_attempt) {
    char * grid[TEST_MAX_ATTEMPTS];
    int propos[] = {1, 2, 3, 4};
    char result[] = {'o', 'o', 'x', '-'};

    init_grid(grid);
    update_grid(grid, 0, propos, result);

    // Check that the grid was updated correctly
    // Format should be "1 2 3 4 | o o x -"
    ASSERT_EQ('1', grid[0][0]);
    ASSERT_EQ('2', grid[0][2]);
    ASSERT_EQ('3', grid[0][4]);
    ASSERT_EQ('4', grid[0][6]);
    ASSERT_EQ('o', grid[0][10]);
    ASSERT_EQ('o', grid[0][12]);
    ASSERT_EQ('x', grid[0][14]);
    ASSERT_EQ('-', grid[0][16]);

    free_grid(grid);
}

UTEST(update_grid, multiple_attempts) {
    char * grid[TEST_MAX_ATTEMPTS];
    int propos1[] = {1, 2, 3, 4};
    char result1[] = {'o', 'x', '-', '-'};
    int propos2[] = {5, 6, 7, 8};
    char result2[] = {'-', '-', '-', '-'};

    init_grid(grid);
    update_grid(grid, 0, propos1, result1);
    update_grid(grid, 1, propos2, result2);

    // Check first attempt
    ASSERT_EQ('1', grid[0][0]);
    ASSERT_EQ('o', grid[0][10]);

    // Check second attempt
    ASSERT_EQ('5', grid[1][0]);
    ASSERT_EQ('6', grid[1][2]);
    ASSERT_EQ('-', grid[1][10]);

    // Check that third attempt is still unmodified
    const char expected[] = "- - - - | - - - -";
    ASSERT_STREQ(expected, grid[2]);

    free_grid(grid);
}

UTEST(update_grid, all_positions) {
    char * grid[TEST_MAX_ATTEMPTS];

    init_grid(grid);

    // Fill all attempts
    for (int attempt = 0; attempt < TEST_MAX_ATTEMPTS; attempt++) {
        int propos[] = {1, 2, 3, 4};
        char result[] = {'o', 'o', 'o', 'o'};
        update_grid(grid, attempt, propos, result);

        ASSERT_EQ('1', grid[attempt][0]);
        ASSERT_EQ('o', grid[attempt][10]);
    }

    free_grid(grid);
}

UTEST(update_grid, with_different_numbers) {
    char * grid[TEST_MAX_ATTEMPTS];
    int propos[] = {9, 8, 7, 6};
    char result[] = {'x', 'x', 'x', 'x'};

    init_grid(grid);
    update_grid(grid, 0, propos, result);

    // Check numbers > 4 (testing with digits 6-9)
    ASSERT_EQ('9', grid[0][0]);
    ASSERT_EQ('8', grid[0][2]);
    ASSERT_EQ('7', grid[0][4]);
    ASSERT_EQ('6', grid[0][6]);
    ASSERT_EQ('x', grid[0][10]);
    ASSERT_EQ('x', grid[0][12]);

    free_grid(grid);
}

UTEST(update_grid, preserves_format_characters) {
    char * grid[TEST_MAX_ATTEMPTS];
    int propos[] = {1, 2, 3, 4};
    char result[] = {'o', 'o', 'o', 'o'};

    init_grid(grid);
    update_grid(grid, 0, propos, result);

    // Check that spaces and pipe are preserved
    ASSERT_EQ(' ', grid[0][1]);
    ASSERT_EQ(' ', grid[0][3]);
    ASSERT_EQ('|', grid[0][8]);
    ASSERT_EQ(' ', grid[0][9]);
    ASSERT_EQ(' ', grid[0][11]);

    free_grid(grid);
}

UTEST(update_grid, last_attempt) {
    char * grid[TEST_MAX_ATTEMPTS];
    int propos[] = {4, 3, 2, 1};
    char result[] = {'-', 'x', 'x', 'o'};

    init_grid(grid);
    update_grid(grid, TEST_MAX_ATTEMPTS - 1, propos, result);

    // Check the last row
    ASSERT_EQ('4', grid[TEST_MAX_ATTEMPTS - 1][0]);
    ASSERT_EQ('3', grid[TEST_MAX_ATTEMPTS - 1][2]);
    ASSERT_EQ('-', grid[TEST_MAX_ATTEMPTS - 1][10]);
    ASSERT_EQ('x', grid[TEST_MAX_ATTEMPTS - 1][12]);
    ASSERT_EQ('o', grid[TEST_MAX_ATTEMPTS - 1][16]);

    free_grid(grid);
}

// Integration test
UTEST(integration, init_update_free_sequence) {
    char * grid[TEST_MAX_ATTEMPTS];

    init_grid(grid);

    int propos[] = {1, 2, 3, 4};
    char result[] = {'o', 'x', '-', 'o'};
    update_grid(grid, 0, propos, result);

    // Verify update worked
    ASSERT_EQ('1', grid[0][0]);
    ASSERT_EQ('o', grid[0][10]);
    ASSERT_EQ('x', grid[0][12]);

    free_grid(grid);

    ASSERT_TRUE(1); // Test passes if no memory errors
}

UTEST_MAIN();
