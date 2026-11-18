#include "../inc/game.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>


// --- MINEFIELD ---
// Helpers
bool _coordinateInBounds(const MineField*, const Coordinate);
uint8_t _countSurroundingMines(const MineField*, const uint8_t);
void _exploreSurroundingSquares(const MineField*, const Coordinate);
void __recursiveExplore(const MineField*, const Coordinate);


// Memory
MineField* createMineField(const uint8_t width, const uint8_t height, const float mineDensity) {
    srand(time(NULL));

    const uint32_t area = width * height;

    MineField* newMineField = malloc(sizeof(*newMineField));

    newMineField->width = width;
    newMineField->height = height;
    newMineField->mineDensity = mineDensity;
    newMineField->totalMines = 0;

    newMineField->fieldData = malloc(sizeof(*newMineField->fieldData) * area);

    for (int i = 0; i < area; ++i) {
        MineSquare currentSquare = (MineSquare) {
            .hasMine=((float) rand() / (float) RAND_MAX) < mineDensity,
            .hasBeenExplored=false,
            .minesSurroundingCount=0,
        };

        if (currentSquare.hasMine) {
            newMineField->totalMines++;
        }

        memcpy(&newMineField->fieldData[i], &currentSquare, sizeof(currentSquare));
    }

    for (int i = 0; i < area; ++i) {
        newMineField->fieldData[i].minesSurroundingCount = _countSurroundingMines(newMineField, i);
    }

    return newMineField;
}


void destroyMinefield(MineField* field) {
    free(field->fieldData);
    free(field);
}


// Logic
GameResponse exploreSquare(MineField* minefield, const uint8_t row, const uint8_t col) {
    const Coordinate coordinate = (Coordinate) {
        .row=row,
        .col=col,
    };
    const uint8_t index = coordinateToIndex(minefield, coordinate);

    const GameResponse moveFailure = (GameResponse) {
        .gameOver=false,
        .gameWon=false,
        .moveSuccess=false,
    };

    const GameResponse moveSuccess = (GameResponse) {
        .gameOver=false,
        .gameWon=false,
        .moveSuccess=true,
    };

    const GameResponse hitMine = (GameResponse) {
        .gameOver=true,
        .gameWon=false,
        .moveSuccess=true,
    };

    const GameResponse clearedField = (GameResponse) {
        .gameOver=true,
        .gameWon=true,
        .moveSuccess=true,
    };

    // Out of bounds
    if (!_coordinateInBounds(minefield, coordinate)) {
        return moveFailure;
    }

    // we can grab a reference to that square now
    const MineSquare selectedSquare = minefield->fieldData[index];

    // Already explored
    if (selectedSquare.hasBeenExplored) {
        return moveFailure;
    }

    // set the real explored value to true
    minefield->fieldData[index].hasBeenExplored = true;

    // YEEEOUCH
    if (selectedSquare.hasMine) {
        return hitMine;
    }

    _exploreSurroundingSquares(minefield, coordinate);

    return moveSuccess;
}


bool toggleMineFlagAndCheckForWin(MineField* minefield, const uint8_t row, const uint8_t col) {
    Coordinate coordinate = (Coordinate) {
        .row=row,
        .col=col,
    };
    uint8_t index = coordinateToIndex(minefield, coordinate);

    if (minefield->fieldData[index].markedWithFlag) {
        minefield->fieldData[index].markedWithFlag = false;
        minefield->flagsUsed--;

    } else if (minefield->flagsUsed < minefield->totalMines) {
        minefield->fieldData[index].markedWithFlag = true;
        minefield->flagsUsed++;

    } else {
        for (int i = 0; i < minefield->width * minefield->height; ++i) {
            const MineSquare currentSquare = minefield->fieldData[i];

            if (currentSquare.hasMine != currentSquare.markedWithFlag) {
                return false;
            }
        }
        return true;
    }

    return false;
}


// Helpers
bool _coordinateInBounds(const MineField* minefield, const Coordinate coordinate) {
    return coordinate.row >= 0 && coordinate.row < minefield->height && coordinate.col >= 0 && coordinate.col < minefield->width;
}


uint8_t _countSurroundingMines(const MineField* minefield, const uint8_t index) {
    Coordinate currentCoordinate = indexToCoordinate(minefield, index);

    const int directions[3] = {-1, 0, 1};

    uint8_t totalMines = 0;

    for (int rowNumber = 0; rowNumber < 3; ++rowNumber) {
        for (int colNumber = 0; colNumber < 3; ++colNumber) {
            const int rowOffset = directions[rowNumber];
            const int colOffset = directions[colNumber];

            const int newRow = currentCoordinate.row + rowOffset;
            const int newCol = currentCoordinate.col + colOffset;

            Coordinate newCoordinate = (Coordinate) {
                .row=newRow,
                .col=newCol,
            };

            if (_coordinateInBounds(minefield, newCoordinate)) {
                if (minefield->fieldData[coordinateToIndex(minefield, newCoordinate)].hasMine) {
                    totalMines++;
                }
            }
        }
    }

    return totalMines;
}


void _exploreSurroundingSquares(const MineField* minefield, const Coordinate coordinate) {
    const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = 0; i < 4; ++i) {
        const int* currentDirection = directions[i];

        const Coordinate newCoordinate = (Coordinate) {
            .row=coordinate.row + currentDirection[0],
            .col=coordinate.col + currentDirection[1],
        };

        if (_coordinateInBounds(minefield, coordinate)) {
            __recursiveExplore(minefield, newCoordinate);
        }
    }
}


void __recursiveExplore(const MineField* minefield, const Coordinate coordinate) {
    const uint32_t index = coordinateToIndex(minefield, coordinate);

    MineSquare currentSquare = minefield->fieldData[index];

    if (currentSquare.hasMine || currentSquare.hasBeenExplored) {
        return;
    }

    minefield->fieldData[index].hasBeenExplored = true;

    if (currentSquare.minesSurroundingCount > 0) {
        return;
    }

    const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = 0; i < 4; ++i) {
        const int* direction = directions[i];

        Coordinate newCoordinate = (Coordinate) {
            .row=coordinate.row + direction[0],
            .col=coordinate.col + direction[1],
        };
    }
}
