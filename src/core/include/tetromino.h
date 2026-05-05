#ifndef TETROMINO_H
#define TETROMINO_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../../api/include/tetris_api.h"
#include "../../shared/game_config.h"
#include "backend_objects.h"

/**
 * @brief Get current active tetromino.
 */
ActiveTetromino_t getCurrentTetrominoInfo();

/**
 * @brief Detect tetromino type from shape matrix.
 */
TetrominoType_t detectTetromino(
    const int shape[TETROMINO_SIZE][TETROMINO_SIZE]);

/**
 * @brief Generate random tetromino shape.
 */
void getRandomTetromino(int dest[TETROMINO_SIZE][TETROMINO_SIZE]);

/**
 * @brief Spawn new tetromino.
 */
void spawnTetromino(int next[TETROMINO_SIZE][TETROMINO_SIZE]);

/**
 * @brief Process tetromino movement.
 */
ActiveTetromino_t tetrominoMovingManager(UserAction_t action);

#endif  // TETROMINO_H