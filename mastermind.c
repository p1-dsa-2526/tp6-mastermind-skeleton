#include "mastermind.h"

void display_comb(int comb[]) {
    for (int i = 0; i < SIZE_COMB; i++) {
        printf("%d ", comb[i]);
    }
    printf("\n");
}

void read_comb(int arr[]) {
    printf("Enter a combination : ");
    for (int i = 0; i < SIZE_COMB; i++) {
        scanf("%d", &arr[i]);
    }
}

void random_comb(int comb[]) {
    for (int i = 0; i < SIZE_COMB; i++) {
        comb[i] = 1 + rand() % NB_COLORS;
    }
}

int present(int comb[], int number) {
    for (int i = 0; i < SIZE_COMB; i++) {
        if (comb[i] == number) {
            return 1;
        }
    }
    return 0;
}

void check_result(int comb[], int propos[], char result[]) {
    // result[i] = x -> good number but wrong place
    // result[i] = o -> good number and good place
    // result[i] = - -> wrong number
    
    for (int i = 0; i < SIZE_COMB; i++) {
        if (comb[i] == propos[i]) {
            result[i] = 'o';
        } else if (present(comb, propos[i])) {
            result[i] = 'x';
        } else {
            result[i] = '-';
        }
    }
}

int well_placed(int comb[], int propos[]) {
    int count = 0;
    for (int i = 0; i < SIZE_COMB; i++) {
        if (comb[i] == propos[i]) {
            count++;
        }
    }
    return count;
}
