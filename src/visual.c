#include <common.h>
#include <visual.h>

#include <stdio.h>
#include <string.h>
#include <termios.h>


// Helpers
void _enableRawInput(void);
void _disableRawInput(void);

void _setBackgroundColor(const uint8_t, const uint8_t, const uint8_t);
void _setForegroundColor(const uint8_t, const uint8_t, const uint8_t);
void _clearFormatting(void);

void _renderRow(const MineField*, const int);
void _renderCell(const MineField*, const int, const int);

void __printCell(const MineSquare);


void renderFrame(const MineField* minefield) {
    for (int row = 0; row < minefield->height; ++row) {
        _renderRow(minefield, row);
    }

    _clearFormatting();
}


UserInput getInput(void) {
    char inputBuffer[32];  
    fgets(inputBuffer, 32, stdin);

    if (strcmp(inputBuffer, "UP\n") == 0) {
        return UP;
    }

    if (strcmp(inputBuffer, "DOWN\n") == 0) {
        return DOWN;
    }

    if (strcmp(inputBuffer, "LEFT\n") == 0) {
        return LEFT;
    }

    if (strcmp(inputBuffer, "RIGHT\n") == 0) {
        return RIGHT;
    }

    if (strcmp(inputBuffer, "EXPLORE\n") == 0) {
        return EXPLORE;
    }

    if (strcmp(inputBuffer, "TOGGLE_FLAG\n") == 0) {
        return TOGGLE_FLAG;
    }

    if (strcmp(inputBuffer, "QUIT\n") == 0) {
        return QUIT;
    }

    printf("Unknown Command!!!\n");
    return QUIT;
}


// Helpers
void _renderRow(const MineField* minefield, const int row) {
    for (int col = 0; col < minefield->width; ++col) {
        _renderCell(minefield, row, col);
    }

    printf("\n");
}


void _renderCell(const MineField* minefield, const int row, const int col) {
    MineSquare currentSquare = minefield->fieldData[coordinateToIndex(minefield, (Coordinate) {.row=row, .col=col})];

    if (!currentSquare.hasBeenExplored) {
        const int colorIndex = (row + col) % 2;
        
        if (colorIndex == 0) {
            _setBackgroundColor(150, 150, 150);
        
        } else {
            _setBackgroundColor(160, 160, 160);
        }

    } else {
        _setBackgroundColor(50, 50, 50);
    }

    Coordinate highlightedSquare = minefield->selectedSquare;

    if (highlightedSquare.row == row && highlightedSquare.col == col) {
        _setBackgroundColor(40, 70, 120);
    }

    __printCell(currentSquare);
}


void _setBackgroundColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    printf("\033[48;2;%hhu;%hhu;%hhum", r, g, b);
}


void _setForegroundColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    printf("\033[38;2;%hhu;%hhu;%hhum", r, g, b);
}


void _clearFormatting(void) {
    printf("\033[0m");
}


void __printCell(const MineSquare square) {
    if (square.markedWithFlag) {
        _setForegroundColor(0, 255, 0);
        printf("F");

        return;
    }

    if (!square.hasBeenExplored) {
        _setForegroundColor(0, 0, 0);
        printf(" ");

        return;
    }
    
    if (square.hasMine) {
        _setForegroundColor(255, 0, 0);
        printf("X");

        return;
    }
}