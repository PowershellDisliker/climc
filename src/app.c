#include "../inc/game.h"
#include "../inc/visual.h"

#include <stdlib.h>


int main(int argc, char* argv[]) {
    uint8_t width = atoi(argv[0]);
    uint8_t height = atoi(argv[1]);
    float mineDensity = atof(argv[2]);

    MineField* minefield = createMineField(width, height, mineDensity);

    printBoard(minefield);

    return 0;
}
