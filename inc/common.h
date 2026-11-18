#pragma once

#include <stdint.h>
#include <stdbool.h>


typedef struct MineSquare {
    const bool hasMine;
    bool hasBeenExplored;
    bool markedWithFlag;
    uint8_t minesSurroundingCount;
} MineSquare;


typedef struct MineField {
    uint8_t width;
    uint8_t height;
    float mineDensity;
    uint8_t totalMines;
    uint8_t flagsUsed;
    MineSquare* fieldData;
} MineField;
