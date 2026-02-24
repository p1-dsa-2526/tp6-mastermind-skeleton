#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mastermind.h"

void rules();
void init_grid(char * grid[]);
void free_grid(char * grid[]);
void update_grid(char * grid[], int attempt, int propos[], char result[]);
void display_grid(char * grid[]);

#endif // DISPLAY_H