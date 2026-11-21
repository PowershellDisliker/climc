#pragma once

#include <common.h>


typedef struct GameResponse {
    bool moveSuccess;
    bool gameOver;
    bool gameWon;
} GameResponse;


// --- MINEFIELD ---
// Memory
MineField* createMineField(const uint8_t, const uint8_t, const float);
void destroyMinefield(MineField*);


// Logic
GameResponse performAction(MineField*, UserInput);
