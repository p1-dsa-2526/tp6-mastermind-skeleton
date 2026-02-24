#include "utest.h"
#include "mastermind.h"
#include <string.h>

#define TEST_SIZE_COMB 4

// display_comb and read_comb are not tested: they write to stdout / read from stdin.
// We would have to do tricks to redirect stdout.

// Test random_comb() function
UTEST(random_comb, generates_valid_numbers) {
    int comb[TEST_SIZE_COMB];
    srand(42); // Fixed seed for reproducibility

    random_comb(comb);

    // Check all numbers are in valid range (1-6 based on the implementation)
    for (int i = 0; i < TEST_SIZE_COMB; i++) {
        ASSERT_TRUE(comb[i] >= 1 && comb[i] <= 6);
    }
}

UTEST(random_comb, generates_different_sequences) {
    int comb1[TEST_SIZE_COMB];
    int comb2[TEST_SIZE_COMB];

    srand(42);
    random_comb(comb1);

    srand(123);
    random_comb(comb2);

    // With different seeds, combinations should likely differ
    // (though not guaranteed, this tests the randomness is working)
    int different = 0;
    for (int i = 0; i < TEST_SIZE_COMB; i++) {
        if (comb1[i] != comb2[i]) {
            different = 1;
            break;
        }
    }
    ASSERT_TRUE(different);
}


// Test well_placed() function
UTEST(well_placed, all_correct) {
    int comb[] = {1, 2, 3, 4};
    int propos[] = {1, 2, 3, 4};
    ASSERT_EQ(4, well_placed(comb, propos));
}

UTEST(well_placed, none_correct) {
    int comb[] = {1, 2, 3, 4};
    int propos[] = {4, 3, 2, 1};
    ASSERT_EQ(0, well_placed(comb, propos));
}

UTEST(well_placed, partial_correct) {
    int comb[] = {1, 2, 3, 4};
    int propos[] = {1, 2, 4, 3};
    ASSERT_EQ(2, well_placed(comb, propos));
}

UTEST(well_placed, one_correct) {
    int comb[] = {1, 2, 3, 4};
    int propos[] = {1, 4, 2, 3};
    ASSERT_EQ(1, well_placed(comb, propos));
}


// Test present() function
UTEST(present, number_exists) {
    int comb[] = {1, 2, 3, 4};
    ASSERT_EQ(1, present(comb, 2));
    ASSERT_EQ(1, present(comb, 1));
    ASSERT_EQ(1, present(comb, 4));
}

UTEST(present, number_not_exists) {
    int comb[] = {1, 2, 3, 4};
    ASSERT_EQ(0, present(comb, 5));
    ASSERT_EQ(0, present(comb, 0));
    ASSERT_EQ(0, present(comb, 6));
}

UTEST(present, with_duplicates) {
    int comb[] = {2, 2, 3, 4};
    ASSERT_EQ(1, present(comb, 2));
    ASSERT_EQ(1, present(comb, 3));
}


// Test check_result() function
UTEST(check_result, all_correct) {
    int comb[] = {1, 2, 3, 4};
    int propos[] = {1, 2, 3, 4};
    char result[TEST_SIZE_COMB];

    check_result(comb, propos, result);

    ASSERT_EQ('o', result[0]);
    ASSERT_EQ('o', result[1]);
    ASSERT_EQ('o', result[2]);
    ASSERT_EQ('o', result[3]);
}

UTEST(check_result, all_wrong) {
    int comb[] = {1, 2, 3, 4};
    int propos[] = {5, 6, 7, 8};
    char result[TEST_SIZE_COMB];

    check_result(comb, propos, result);

    ASSERT_EQ('-', result[0]);
    ASSERT_EQ('-', result[1]);
    ASSERT_EQ('-', result[2]);
    ASSERT_EQ('-', result[3]);
}

UTEST(check_result, wrong_position) {
    int comb[] = {1, 2, 3, 4};
    int propos[] = {4, 3, 2, 1};
    char result[TEST_SIZE_COMB];

    check_result(comb, propos, result);

    ASSERT_EQ('x', result[0]);
    ASSERT_EQ('x', result[1]);
    ASSERT_EQ('x', result[2]);
    ASSERT_EQ('x', result[3]);
}

UTEST(check_result, mixed) {
    int comb[] = {1, 2, 3, 4};
    int propos[] = {1, 3, 2, 5};
    char result[TEST_SIZE_COMB];

    check_result(comb, propos, result);

    ASSERT_EQ('o', result[0]); // 1 is correct position
    ASSERT_EQ('x', result[1]); // 3 is in combination but wrong position
    ASSERT_EQ('x', result[2]); // 2 is in combination but wrong position
    ASSERT_EQ('-', result[3]); // 5 is not in combination
}

UTEST(check_result, partial_match) {
    int comb[] = {1, 2, 3, 4};
    int propos[] = {1, 2, 5, 6};
    char result[TEST_SIZE_COMB];

    check_result(comb, propos, result);

    ASSERT_EQ('o', result[0]);
    ASSERT_EQ('o', result[1]);
    ASSERT_EQ('-', result[2]);
    ASSERT_EQ('-', result[3]);
}


// Test edge cases
UTEST(check_result, duplicate_numbers_in_comb) {
    int comb[] = {1, 1, 2, 3};
    int propos[] = {1, 2, 3, 4};
    char result[TEST_SIZE_COMB];

    check_result(comb, propos, result);

    ASSERT_EQ('o', result[0]); // 1 is correct
    ASSERT_EQ('x', result[1]); // 2 is present but wrong position
    ASSERT_EQ('x', result[2]); // 3 is present but wrong position
    ASSERT_EQ('-', result[3]); // 4 is not present
}

UTEST(check_result, duplicate_numbers_in_propos) {
    int comb[] = {1, 2, 3, 4};
    int propos[] = {1, 1, 1, 1};
    char result[TEST_SIZE_COMB];

    check_result(comb, propos, result);

    ASSERT_EQ('o', result[0]); // First 1 is correctly placed
    ASSERT_EQ('x', result[1]); // Second 1 is present (at position 0)
    ASSERT_EQ('x', result[2]); // Third 1 is present
    ASSERT_EQ('x', result[3]); // Fourth 1 is present
}

UTEST_MAIN();
