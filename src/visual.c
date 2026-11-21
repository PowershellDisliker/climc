#include <common.h>
#include <visual.h>

#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>


// Helpers
void _enableRawInput(void);
void _disableRawInput(void);

void _setBackgroundColor(const uint8_t, const uint8_t, const uint8_t);
void _setForegroundColor(const uint8_t, const uint8_t, const uint8_t);
void _clearFormatting(void);

void _renderRow(const MineField*, const int);
void _renderCell(const MineField*, const int, const int);

void __printCell(const MineSquare);


// Terminal State Object
struct termios orig_termios;


// Logic
void setupRendering(void) {
    _enableRawInput();
}


void teardownRendering(void) {
    _disableRawInput();
}


void renderFrame(const MineField* minefield) {
    printf("\x1b[H");

    for (int row = 0; row < minefield->height; ++row) {
        _renderRow(minefield, row);
    }

    _clearFormatting();
}


UserInput getInput(void) {
    char pressedKey;

    read(STDIN_FILENO, &pressedKey, 1);

    switch (pressedKey) {
        case 'w':
            return UP;
        
        case 's':
            return DOWN;

        case 'a':
            return LEFT;

        case 'd':
            return RIGHT;

        case 'e':
            return EXPLORE;

        case 'f':
            return TOGGLE_FLAG;

        case 'q':
            return QUIT;

        default:
            return UP;
    }
}


// Helpers
void _enableRawInput(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    
    atexit(_disableRawInput);

    struct termios raw = orig_termios;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    // raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    write(STDOUT_FILENO, "\x1b[?1049h", 8);
}


void _disableRawInput(void) {
    write(STDOUT_FILENO, "\x1b[?1049l\x1b[?25h", 14);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}


void _renderRow(const MineField* minefield, const int row) {
    for (int col = 0; col < minefield->width; ++col) {
        _renderCell(minefield, row, col);
    }

    _clearFormatting();
    printf("\n");
}


void _renderCell(const MineField* minefield, const int row, const int col) {
    MineSquare currentSquare = minefield->fieldData[coordinateToIndex(minefield, (Coordinate) {.row=row, .col=col})];

    if (!currentSquare.hasBeenExplored) {
        const int colorIndex = (row + col) % 2;
        
        if (colorIndex == 0) {
            _setBackgroundColor(140, 140, 140);
        
        } else {
            _setBackgroundColor(160, 160, 160);
        }

    } else {
        _setBackgroundColor(120, 120, 120);
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
        printf(" F ");

        return;
    }

    if (!square.hasBeenExplored) {
        _setForegroundColor(0, 0, 0);
        printf("   ");

        return;
    }
    
    if (square.hasMine) {
        _setForegroundColor(255, 0, 0);
        printf(" X ");

        return;
    }

    _setForegroundColor(10, 10, 10);
    printf(" %hhu ", square.minesSurroundingCount);
}