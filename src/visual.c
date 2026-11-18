#include "../inc/visual.h"

#include <stdio.h>


void printBoard(const MineField* minefield) {
    for (int row = 0; row < minefield->height; ++row) {
        printf("|");

        for (int col = 0; col < minefield->width; ++col) {
            printf("");
        }

        printf("|\n");
    }
}
