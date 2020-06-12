#ifndef GRID_H
#define GRID_H


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "tetriminos.h"

typedef int colour_t;
typedef colour_t **grid_t;

#define GWIDTH 10
#define GHEIGHT 22 //Top 2 rows are hidden

grid_t initGrid();

void printGrid(grid_t grid);

colour_t *getSquare(grid_t grid, position_t p);

void freeGrid(grid_t grid);

#endif