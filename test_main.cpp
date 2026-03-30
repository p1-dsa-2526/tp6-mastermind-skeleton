#include <catch2/catch_test_macros.hpp>

extern "C" {
#include "mastermind.h"
#include "display.h"
}

#include <cstring>

// ============================================================================
// Tests for mastermind functions
// ============================================================================

// display_comb and read_comb are not tested: they write to stdout / read from stdin.

#define TEST_SIZE_COMB 4

TEST_CASE("mastermind", "[mastermind]") {
    SECTION("random_comb") {
        SECTION("generates valid numbers in range 1-6") {
            int comb[TEST_SIZE_COMB] = {};
            srand(42); // Fixed seed for reproducibility

            random_comb(comb);

            for (int i = 0; i < TEST_SIZE_COMB; i++) {
                REQUIRE(comb[i] >= 1);
                REQUIRE(comb[i] <= 6);
            }
        }

        SECTION("generates different sequences with different seeds") {
            int comb1[TEST_SIZE_COMB] = {};
            int comb2[TEST_SIZE_COMB] = {};

            srand(42);
            random_comb(comb1);

            srand(123);
            random_comb(comb2);

            // With different seeds, combinations should likely differ
            bool different = false;
            for (int i = 0; i < TEST_SIZE_COMB; i++) {
                if (comb1[i] != comb2[i]) {
                    different = true;
                    break;
                }
            }
            REQUIRE(different);
        }
    }

    SECTION("well_placed") {
        SECTION("all correct") {
            int comb[] = {1, 2, 3, 4};
            int propos[] = {1, 2, 3, 4};
            REQUIRE(well_placed(comb, propos) == 4);
        }

        SECTION("none correct") {
            int comb[] = {1, 2, 3, 4};
            int propos[] = {4, 3, 2, 1};
            REQUIRE(well_placed(comb, propos) == 0);
        }

        SECTION("partial correct") {
            int comb[] = {1, 2, 3, 4};
            int propos[] = {1, 2, 4, 3};
            REQUIRE(well_placed(comb, propos) == 2);
        }

        SECTION("one correct") {
            int comb[] = {1, 2, 3, 4};
            int propos[] = {1, 4, 2, 3};
            REQUIRE(well_placed(comb, propos) == 1);
        }
    }

    SECTION("present") {
        SECTION("number exists") {
            int comb[] = {1, 2, 3, 4};
            REQUIRE(present(comb, 2) == 1);
            REQUIRE(present(comb, 1) == 1);
            REQUIRE(present(comb, 4) == 1);
        }

        SECTION("number does not exist") {
            int comb[] = {1, 2, 3, 4};
            REQUIRE(present(comb, 5) == 0);
            REQUIRE(present(comb, 0) == 0);
            REQUIRE(present(comb, 6) == 0);
        }

        SECTION("with duplicates") {
            int comb[] = {2, 2, 3, 4};
            REQUIRE(present(comb, 2) == 1);
            REQUIRE(present(comb, 3) == 1);
        }
    }

    SECTION("check_result") {
        SECTION("all correct") {
            int comb[] = {1, 2, 3, 4};
            int propos[] = {1, 2, 3, 4};
            char result[TEST_SIZE_COMB] = {};

            check_result(comb, propos, result);

            REQUIRE(result[0] == 'o');
            REQUIRE(result[1] == 'o');
            REQUIRE(result[2] == 'o');
            REQUIRE(result[3] == 'o');
        }

        SECTION("all wrong") {
            int comb[] = {1, 2, 3, 4};
            int propos[] = {5, 6, 7, 8};
            char result[TEST_SIZE_COMB] = {};

            check_result(comb, propos, result);

            REQUIRE(result[0] == '-');
            REQUIRE(result[1] == '-');
            REQUIRE(result[2] == '-');
            REQUIRE(result[3] == '-');
        }

        SECTION("wrong position") {
            int comb[] = {1, 2, 3, 4};
            int propos[] = {4, 3, 2, 1};
            char result[TEST_SIZE_COMB] = {};

            check_result(comb, propos, result);

            REQUIRE(result[0] == 'x');
            REQUIRE(result[1] == 'x');
            REQUIRE(result[2] == 'x');
            REQUIRE(result[3] == 'x');
        }

        SECTION("mixed results") {
            int comb[] = {1, 2, 3, 4};
            int propos[] = {1, 3, 2, 5};
            char result[TEST_SIZE_COMB] = {};

            check_result(comb, propos, result);

            REQUIRE(result[0] == 'o'); // 1 is correct position
            REQUIRE(result[1] == 'x'); // 3 is in combination but wrong position
            REQUIRE(result[2] == 'x'); // 2 is in combination but wrong position
            REQUIRE(result[3] == '-'); // 5 is not in combination
        }

        SECTION("partial match") {
            int comb[] = {1, 2, 3, 4};
            int propos[] = {1, 2, 5, 6};
            char result[TEST_SIZE_COMB] = {};

            check_result(comb, propos, result);

            REQUIRE(result[0] == 'o');
            REQUIRE(result[1] == 'o');
            REQUIRE(result[2] == '-');
            REQUIRE(result[3] == '-');
        }

        SECTION("duplicate numbers in combination") {
            int comb[] = {1, 1, 2, 3};
            int propos[] = {1, 2, 3, 4};
            char result[TEST_SIZE_COMB] = {};

            check_result(comb, propos, result);

            REQUIRE(result[0] == 'o'); // 1 is correct
            REQUIRE(result[1] == 'x'); // 2 is present but wrong position
            REQUIRE(result[2] == 'x'); // 3 is present but wrong position
            REQUIRE(result[3] == '-'); // 4 is not present
        }

        SECTION("duplicate numbers in proposal") {
            int comb[] = {1, 2, 3, 4};
            int propos[] = {1, 1, 1, 1};
            char result[TEST_SIZE_COMB] = {};

            check_result(comb, propos, result);

            REQUIRE(result[0] == 'o'); // First 1 is correctly placed
            REQUIRE(result[1] == 'x'); // Second 1 is present (at position 0)
            REQUIRE(result[2] == 'x'); // Third 1 is present
            REQUIRE(result[3] == 'x'); // Fourth 1 is present
        }
    }
}

// ============================================================================
// Tests for display functions
// ============================================================================

#define TEST_MAX_ATTEMPTS 8

TEST_CASE("display", "[display]") {
    SECTION("init_grid") {
        SECTION("allocates memory") {
            char * grid[TEST_MAX_ATTEMPTS] = {};

            init_grid(grid);

            for (int i = 0; i < TEST_MAX_ATTEMPTS; i++) {
                REQUIRE(grid[i] != nullptr);
            }

            free_grid(grid);
        }

        SECTION("initializes with correct format") {
            char * grid[TEST_MAX_ATTEMPTS] = {};

            init_grid(grid);

            const char expected[] = "- - - - | - - - -";
            for (int i = 0; i < TEST_MAX_ATTEMPTS; i++) {
                REQUIRE(grid[i] != nullptr);
                REQUIRE(strcmp(grid[i], expected) == 0);
            }

            free_grid(grid);
        }
    }

    SECTION("free_grid") {
        SECTION("deallocates memory without crash") {
            char * grid[TEST_MAX_ATTEMPTS] = {};

            init_grid(grid);
            free_grid(grid);

            REQUIRE(true); // If we get here without crash, test passes
        }
    }

    SECTION("update_grid") {
        SECTION("first attempt") {
            char * grid[TEST_MAX_ATTEMPTS] = {};
            int propos[] = {1, 2, 3, 4};
            char result[] = {'o', 'o', 'x', '-'};

            init_grid(grid);
            REQUIRE(grid[0] != nullptr);
            update_grid(grid, 0, propos, result);

            REQUIRE(grid[0][0] == '1');
            REQUIRE(grid[0][2] == '2');
            REQUIRE(grid[0][4] == '3');
            REQUIRE(grid[0][6] == '4');
            REQUIRE(grid[0][10] == 'o');
            REQUIRE(grid[0][12] == 'o');
            REQUIRE(grid[0][14] == 'x');
            REQUIRE(grid[0][16] == '-');

            free_grid(grid);
        }

        SECTION("multiple attempts") {
            char * grid[TEST_MAX_ATTEMPTS] = {};
            int propos1[] = {1, 2, 3, 4};
            char result1[] = {'o', 'x', '-', '-'};
            int propos2[] = {5, 6, 7, 8};
            char result2[] = {'-', '-', '-', '-'};

            init_grid(grid);
            REQUIRE(grid[0] != nullptr);
            REQUIRE(grid[1] != nullptr);
            REQUIRE(grid[2] != nullptr);
            update_grid(grid, 0, propos1, result1);
            update_grid(grid, 1, propos2, result2);

            REQUIRE(grid[0][0] == '1');
            REQUIRE(grid[0][10] == 'o');

            REQUIRE(grid[1][0] == '5');
            REQUIRE(grid[1][2] == '6');
            REQUIRE(grid[1][10] == '-');

            // Third attempt should still be unmodified
            const char expected[] = "- - - - | - - - -";
            REQUIRE(strcmp(grid[2], expected) == 0);

            free_grid(grid);
        }

        SECTION("all positions") {
            char * grid[TEST_MAX_ATTEMPTS] = {};

            init_grid(grid);
            for (int i = 0; i < TEST_MAX_ATTEMPTS; i++) {
                REQUIRE(grid[i] != nullptr);
            }

            for (int attempt = 0; attempt < TEST_MAX_ATTEMPTS; attempt++) {
                int propos[] = {1, 2, 3, 4};
                char result[] = {'o', 'o', 'o', 'o'};
                update_grid(grid, attempt, propos, result);

                REQUIRE(grid[attempt][0] == '1');
                REQUIRE(grid[attempt][10] == 'o');
            }

            free_grid(grid);
        }

        SECTION("with different numbers") {
            char * grid[TEST_MAX_ATTEMPTS] = {};
            int propos[] = {9, 8, 7, 6};
            char result[] = {'x', 'x', 'x', 'x'};

            init_grid(grid);
            REQUIRE(grid[0] != nullptr);
            update_grid(grid, 0, propos, result);

            REQUIRE(grid[0][0] == '9');
            REQUIRE(grid[0][2] == '8');
            REQUIRE(grid[0][4] == '7');
            REQUIRE(grid[0][6] == '6');
            REQUIRE(grid[0][10] == 'x');
            REQUIRE(grid[0][12] == 'x');

            free_grid(grid);
        }

        SECTION("preserves format characters") {
            char * grid[TEST_MAX_ATTEMPTS] = {};
            int propos[] = {1, 2, 3, 4};
            char result[] = {'o', 'o', 'o', 'o'};

            init_grid(grid);
            REQUIRE(grid[0] != nullptr);
            update_grid(grid, 0, propos, result);

            REQUIRE(grid[0][1] == ' ');
            REQUIRE(grid[0][3] == ' ');
            REQUIRE(grid[0][8] == '|');
            REQUIRE(grid[0][9] == ' ');
            REQUIRE(grid[0][11] == ' ');

            free_grid(grid);
        }

        SECTION("last attempt") {
            char * grid[TEST_MAX_ATTEMPTS] = {};
            int propos[] = {4, 3, 2, 1};
            char result[] = {'-', 'x', 'x', 'o'};

            init_grid(grid);
            REQUIRE(grid[TEST_MAX_ATTEMPTS - 1] != nullptr);
            update_grid(grid, TEST_MAX_ATTEMPTS - 1, propos, result);

            REQUIRE(grid[TEST_MAX_ATTEMPTS - 1][0] == '4');
            REQUIRE(grid[TEST_MAX_ATTEMPTS - 1][2] == '3');
            REQUIRE(grid[TEST_MAX_ATTEMPTS - 1][10] == '-');
            REQUIRE(grid[TEST_MAX_ATTEMPTS - 1][12] == 'x');
            REQUIRE(grid[TEST_MAX_ATTEMPTS - 1][16] == 'o');

            free_grid(grid);
        }
    }

    SECTION("integration: init, update, free sequence") {
        char * grid[TEST_MAX_ATTEMPTS] = {};

        init_grid(grid);
        REQUIRE(grid[0] != nullptr);

        int propos[] = {1, 2, 3, 4};
        char result[] = {'o', 'x', '-', 'o'};
        update_grid(grid, 0, propos, result);

        REQUIRE(grid[0][0] == '1');
        REQUIRE(grid[0][10] == 'o');
        REQUIRE(grid[0][12] == 'x');

        free_grid(grid);
    }
}
