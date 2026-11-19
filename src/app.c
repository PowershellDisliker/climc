/*
Minesweeper clone

Author: Ryley Turner
*/

#include "../inc/game.h"
#include "../inc/visual.h"

#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 128


int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: minesweeper <width> <height> <mine density>\n");
        return 1;
    }

    uint8_t width = atoi(argv[1]);
    uint8_t height = atoi(argv[2]);
    float mineDensity = atof(argv[3]);

    MineField* minefield = createMineField(width, height, mineDensity);

    GameResponse recentResponse = (GameResponse) {
        .gameOver=false,
        .gameWon=false,
        .moveSuccess=false,
    };

    char inputBuffer[BUFFER_SIZE];

    while (!recentResponse.gameOver) {
        printBoard(minefield);

        char mode = '0';
        uint8_t userRow, userCol = 0;

        while (true) {
            printf("> \n");

            if (fgets(inputBuffer, BUFFER_SIZE, stdin) != NULL) {
                if (sscanf(inputBuffer, "%c %hhu %hhu", &mode, &userRow, &userCol) == 3) {
                    break;
                }
            }
        }

        if (mode == 'x') {
            recentResponse = exploreSquare(minefield, userRow, userCol);
        } else if (mode == 'f') {
            toggleMineFlagAndCheckForWin(minefield, userRow, userCol);
        } else {
            printf("Mode not recognized\n");
        }
    }

    printBoard(minefield);
    destroyMinefield(minefield);

    return 0;
}
