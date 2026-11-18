/*
Minesweeper clone

Author: Ryley Turner
*/

#include "../inc/game.h"
#include "../inc/visual.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: minesweeper <width> <height> <mine density>\n");
        return 1;
    }

    uint8_t width = atoi(argv[1]);
    uint8_t height = atoi(argv[2]);
    float mineDensity = atof(argv[3]);

    MineField* minefield = createMineField(width, height, mineDensity);

    printBoard(minefield);

    return 0;
}
