/*
  =========================================
  Game field state and collision management
  =========================================
*/

#include "include/field_control.h"

// ================
// Internal objects

static Field_t WITHOUT_active_tetromino;
static Field_t WITH_active_tetromino;
static bool tetromino_merged = FALSE;

/**
 * @brief Merge active tetromino into the static field.
 *
 * After merging, the tetromino becomes part of the field and stops moving.
 */

static void mergeTetrominoToField(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE],
                                  int x, int y) {
  tetromino_merged = TRUE;

  for (int ty = 0; ty < TETROMINO_SIZE; ty++) {
    for (int tx = 0; tx < TETROMINO_SIZE; tx++) {
      if (tetromino[ty][tx] != 0) {
        int field_x = x + tx;
        int field_y = y + ty;

        if (field_x >= 0 && field_x < FIELD_WIDTH && field_y >= 0 &&
            field_y < FIELD_HEIGHT) {
          WITHOUT_active_tetromino.field[field_y][field_x] = tetromino[ty][tx];
        }
      }
    }
  }

  memcpy(&WITH_active_tetromino, &WITHOUT_active_tetromino, sizeof(Field_t));
}

/**
 * @brief Get field with active tetromino applied.
 */
Field_t getCurrentFieldWithActiveTetromino() { return WITH_active_tetromino; }

/**
 * @brief Check if current tetromino has merged.
 */
bool checkMerge() { return tetromino_merged; }

/**
 * @brief Reset merge state.
 */
void resetMergeStatus() { tetromino_merged = FALSE; }

/**
 * @brief Initialize field state.
 */
void fieldInit() {
  memset(&WITHOUT_active_tetromino, 0, sizeof(Field_t));
  memset(&WITH_active_tetromino, 0, sizeof(Field_t));
  tetromino_merged = FALSE;
}

/**
 * @brief Check if a new tetromino can be spawned.
 *
 * @return true if game over condition is met
 */
bool checkGameOverCondition(int next[TETROMINO_SIZE][TETROMINO_SIZE]) {
  return !canPlace(next, TETROMINO_SPAWN_X, TETROMINO_SPAWN_Y);
}

/**
 * @brief Overlay active tetromino onto the field.
 */
void updateFieldWithActiveTetromino(
    int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
  if (tetromino_merged) {
    memcpy(&WITH_active_tetromino, &WITHOUT_active_tetromino, sizeof(Field_t));

  } else {
    memcpy(&WITH_active_tetromino, &WITHOUT_active_tetromino, sizeof(Field_t));

    for (int ty = 0; ty < TETROMINO_SIZE; ty++) {
      for (int tx = 0; tx < TETROMINO_SIZE; tx++) {
        if (tetromino[ty][tx] != 0) {
          int field_x = x + tx;
          int field_y = y + ty;

          if (field_x >= 0 && field_x < FIELD_WIDTH && field_y >= 0 &&
              field_y < FIELD_HEIGHT) {
            WITH_active_tetromino.field[field_y][field_x] = tetromino[ty][tx];
          }
        }
      }
    }
  }
}

/**
 * @brief Remove completed lines and shift field down.
 *
 * @return Number of cleared lines
 */
int clearCompletedLines() {
  int lines_cleared = 0;

  for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
    bool line_complete = TRUE;

    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (WITHOUT_active_tetromino.field[y][x] == 0) {
        line_complete = FALSE;
        x = FIELD_WIDTH;
      }
    }
    if (line_complete) {
      lines_cleared++;
      for (int ny = y; ny > 0; ny--) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
          WITHOUT_active_tetromino.field[ny][x] =
              WITHOUT_active_tetromino.field[ny - 1][x];
        }
      }
      for (int x = 0; x < FIELD_WIDTH; x++) {
        WITHOUT_active_tetromino.field[0][x] = 0;
      }
      y++;
    }
  }
  memcpy(&WITH_active_tetromino, &WITHOUT_active_tetromino, sizeof(Field_t));

  return lines_cleared;
}

/**
 * @brief Check if tetromino can move right.
 */
bool canMoveRight(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
  bool res = TRUE;
  if (tetromino_merged) {
    res = FALSE;
  } else {
    for (int ty = 0; ty < TETROMINO_SIZE; ty++) {
      for (int tx = 0; tx < TETROMINO_SIZE; tx++) {
        if (tetromino[ty][tx] != 0) {
          int field_x = x + tx + 1;
          int field_y = y + ty;

          if (field_x >= FIELD_WIDTH) {
            res = FALSE;
          }

          if (field_y >= 0 && field_y < FIELD_HEIGHT &&
              WITHOUT_active_tetromino.field[field_y][field_x] != 0) {
            res = FALSE;
          }
        }
      }
    }
  }

  return res;
}

/**
 * @brief Check if tetromino can move left.
 */
bool canMoveLeft(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
  bool res = TRUE;
  if (tetromino_merged) {
    res = FALSE;
  } else {
    for (int ty = 0; ty < TETROMINO_SIZE; ty++) {
      for (int tx = 0; tx < TETROMINO_SIZE; tx++) {
        if (tetromino[ty][tx] != 0) {
          int field_x = x + tx - 1;
          int field_y = y + ty;

          if (field_x < 0) {
            res = FALSE;
          }

          if (field_y >= 0 && field_y < FIELD_HEIGHT &&
              WITHOUT_active_tetromino.field[field_y][field_x] != 0) {
            res = FALSE;
          }
        }
      }
    }
  }

  return res;
}

/**
 * @brief Check if tetromino can move down.
 *
 * Also triggers merge if movement is no longer possible.
 */
bool canMoveDown(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
  bool res = TRUE;

  if (tetromino_merged) {
    res = FALSE;
  } else {
    for (int ty = 0; ty < TETROMINO_SIZE; ty++) {
      for (int tx = 0; tx < TETROMINO_SIZE; tx++) {
        if (tetromino[ty][tx] != 0) {
          int field_x = x + tx;
          int field_y = y + ty + 1;

          if (field_y >= FIELD_HEIGHT) {
            mergeTetrominoToField(tetromino, x, y);
            res = FALSE;
          }

          if (field_y >= 0 &&
              WITHOUT_active_tetromino.field[field_y][field_x] != 0) {
            mergeTetrominoToField(tetromino, x, y);
            res = FALSE;
          }
        }
      }
    }
  }

  return res;
}

/**
 * @brief Drop tetromino to the lowest possible position.
 *
 * Note: Currently does not update position externally.
 */

void dropTetromino(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x,
                   int y) {
  if (!tetromino_merged) {
    int new_y = y;
    while (canMoveDown(tetromino, x, new_y)) {
      new_y++;
    }
  }
}

/**
 * @brief Check if tetromino can be placed at given position.
 */
bool canPlace(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
  bool res = TRUE;
  for (int ty = 0; ty < TETROMINO_SIZE; ty++) {
    for (int tx = 0; tx < TETROMINO_SIZE; tx++) {
      if (tetromino[ty][tx] != 0) {
        int field_x = x + tx;
        int field_y = y + ty;

        if (field_x < 0 || field_x >= FIELD_WIDTH || field_y >= FIELD_HEIGHT) {
          res = FALSE;
        }

        if (field_y < 0) {
          continue;
        }

        if (WITHOUT_active_tetromino.field[field_y][field_x] != 0) {
          res = FALSE;
        }
      }
    }
  }
  return res;
}