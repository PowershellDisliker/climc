#include "../inc/visual.h"
#include "../inc/common.h"

#include <stdio.h>
#include <termios.h>


void printBoard(const MineField* minefield) {
    for (int row = 0; row < minefield->height; ++row) {

        for (int col = 0; col < minefield->width; ++col) {
            Coordinate coordinate = (Coordinate) {
                .row=row,
                .col=col,
            };

            MineSquare currentSquare = minefield->fieldData[coordinateToIndex(minefield, coordinate)];

            if (currentSquare.hasMine) {
                printf("| X ");
            } else if (currentSquare.minesSurroundingCount > 0) {
                printf("| %u ", currentSquare.minesSurroundingCount);
            } else {
                printf("|   ");
            }

        }

        printf("|\n");
    }
}
