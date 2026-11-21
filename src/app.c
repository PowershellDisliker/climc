/*
CLI Minesweeper Clone (CLIMC)

Author: Ryley Turner
*/

#include <game.h>
#include <visual.h>
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
        renderFrame(minefield);

        UserInput input = getInput();

        recentResponse = performAction(minefield, input);
    }

    renderFrame(minefield);
    destroyMinefield(minefield);

    return 0;
}
