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


typedef struct GameResponse {
    const bool moveSuccess;
    const bool gameOver;
    const bool gameWon;
} GameResponse;

// --- MINEFIELD ---
// Memory
MineField* createMineField(const uint8_t, const uint8_t, const float);
void destroyMinefield(MineField*);

// Logic
GameResponse exploreSquare(MineField*, const uint8_t, const uint8_t);
bool toggleMineFlagAndCheckForWin(MineField*, const uint8_t, const uint8_t);
