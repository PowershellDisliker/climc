/*
CLI Minesweeper Clone (CLIMC)

Author: Ryley Turner
*/

#include <game.h>
#include <visual.h>


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 128

typedef struct Arguments {
    uint8_t width;
    uint8_t height;
    float mineDensity;
} Arguments;


bool _validateArguments(int, char**);
Arguments _getArguments(int, char**);


int main(int argc, char* argv[]) {
    if (!_validateArguments(argc, argv)) {
        return 1;
    }

    Arguments parsedArgs = _getArguments(argc, argv);

    MineField* minefield = createMineField(parsedArgs.width, parsedArgs.height, parsedArgs.mineDensity);

    GameResponse recentResponse = (GameResponse) {
        .gameOver=false,
        .gameWon=false,
        .moveSuccess=false,
    };

    setupRendering();

    while (!recentResponse.gameOver) {
        renderFrame(minefield);

        UserInput input = getInput();

        recentResponse = performAction(minefield, input);
    }

    renderFrame(minefield);
    teardownRendering();
    destroyMinefield(minefield);

    return 0;
}


bool _validateArguments(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: minesweeper <width> <height> <mine density>\n");
        return false;
    }

    return true;
}


Arguments _getArguments(int argc, char* argv[]) {
    return (Arguments) {
        .width=atoi(argv[1]),
        .height=atoi(argv[2]),
        .mineDensity=atof(argv[3])
    };
}
