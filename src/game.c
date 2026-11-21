#include <common.h>
#include <game.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>


// --- MINEFIELD ---
// Helpers
GameResponse _toggleFlag(MineField*);
GameResponse _explore(MineField*);

bool _coordinateInBounds(const MineField*, const Coordinate);
uint8_t _countSurroundingMines(const MineField*, const uint8_t);

uint8_t _countUsedFlags(const MineField*);
GameResponse _checkForWin(const MineField*);

void _exploreSurroundingSquares(const MineField*, const Coordinate);
void __recursiveExplore(const MineField*, const Coordinate);


// Memory
MineField* createMineField(const uint8_t width, const uint8_t height, const float mineDensity) {
    srand(time(NULL));

    const uint32_t area = width * height;

    MineField* newMineField = malloc(sizeof(*newMineField));

    newMineField->width = width;
    newMineField->height = height;
    newMineField->selectedSquare = (Coordinate) {.row=0, .col=0};
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

        // bad... probably. lots of memcpys
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
GameResponse performAction(MineField* minefield, UserInput input) {
    const Coordinate current = minefield->selectedSquare;
    
    switch (input) {
        case UP:
            if (current.row < 0) {
                minefield->selectedSquare = (Coordinate) {.row=minefield->height - 1, .col=current.col};
                break;
            }

            minefield->selectedSquare = (Coordinate) {.row=current.row - 1, .col=current.col};
            break;

        case DOWN:
            if (current.row >= minefield->height) {
                minefield->selectedSquare = (Coordinate) {.row=0, .col=current.col};
                break;
            }

            minefield->selectedSquare = (Coordinate) {.row=current.row + 1, .col=current.col};
            break;

        case LEFT:
            if (current.col < 0) {
                minefield->selectedSquare = (Coordinate) {.row=current.row, .col=minefield->width - 1};
                break;
            }

            minefield->selectedSquare = (Coordinate) {.row=current.row, .col=current.col - 1};
            break;

        case RIGHT:
            if (current.row >= minefield->width) {
                minefield->selectedSquare = (Coordinate) {.row=current.row, .col=0};
                break;
            }

            minefield->selectedSquare = (Coordinate) {.row=current.row, .col=current.col + 1};
            break;

        case TOGGLE_FLAG:
            return _toggleFlag(minefield);
            break;

        case EXPLORE:
            return _explore(minefield);
            break;

        case QUIT:
            return (GameResponse) {
                .gameOver=true,
                .gameWon=false,
                .moveSuccess=false,
            };

            break;
    }

    return (GameResponse) {
        .gameOver=false,
        .gameWon=false,
        .moveSuccess=true,
    };
}


GameResponse _explore(MineField* minefield) {
    const Coordinate coordinate = minefield->selectedSquare;
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

    if (selectedSquare.minesSurroundingCount == 0) {
        _exploreSurroundingSquares(minefield, coordinate);
    }

    return _checkForWin(minefield);
}


GameResponse _toggleFlag(MineField* minefield) {
    const Coordinate coordinate = minefield->selectedSquare;
    const uint8_t index = coordinateToIndex(minefield, coordinate);

    minefield->fieldData[index].markedWithFlag = !minefield->fieldData[index].markedWithFlag;

    return _checkForWin(minefield);
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


GameResponse _checkForWin(const MineField* minefield) {
    const GameResponse winResponse = (GameResponse) {
        .gameOver=true,
        .gameWon=true,
        .moveSuccess=true,
    };

    const GameResponse inertResponse = (GameResponse) {
        .gameOver=false,
        .gameWon=false,
        .moveSuccess=true,
    };
    
    for (int i = 0; i < minefield->width * minefield->height; ++i) {
        const MineSquare currentSquare = minefield->fieldData[i];

        if (!currentSquare.hasBeenExplored || currentSquare.hasMine != currentSquare.markedWithFlag) {
            return inertResponse;
        }
    }

    return winResponse;
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

        if (_coordinateInBounds(minefield, newCoordinate)) {
            __recursiveExplore(minefield, newCoordinate);
        }
    }
}
