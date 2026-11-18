#include "../inc/visual.h"

#include <stdio.h>


void printBoard(const MineField* minefield) {
    printf("%u x %u: %f\n", minefield->width, minefield->height, minefield->mineDensity);
}
