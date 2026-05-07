/**
 * @file field_control.h
 * @brief Field state management and collision detection.
 */

#ifndef FIELD_CONTROL_H
#define FIELD_CONTROL_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../../game_model/game_model.h"
#include "core_context.h"

// ============================================
// Field state (used by game_info)
// ============================================

Field_t getCurrentFieldWithActiveTetromino();
bool checkMerge();
void resetMergeStatus();
void fieldInit();
bool checkGameOverCondition(int next[TETROMINO_SIZE][TETROMINO_SIZE]);
void updateFieldWithActiveTetromino(
    int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y);
int clearCompletedLines();

// ============================================
// Movement checks (used by tetromino)
// ============================================

bool canMoveRight(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y);
bool canMoveLeft(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y);
bool canMoveDown(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y);
void dropTetromino(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y);
bool canPlace(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y);

#endif  // FIELD_CONTROL_H