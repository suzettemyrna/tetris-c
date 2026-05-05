/*
  ==========================================
  Tetromino spawning and movement management
  ==========================================
*/

#include "include/tetromino.h"

#include "include/field_control.h"

// ================
// Internal objects

static ActiveTetromino_t the_piece;
static Tetromino_t tetrominoes[] = {
    // I // cyan
    {.type = TETROMINO_I,
     .shape = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     .pivot_x = 1.5,
     .pivot_y = 1.5},
    // J // blue
    {.type = TETROMINO_J,
     .shape = {{2, 0, 0, 0}, {2, 2, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     .pivot_x = 1,
     .pivot_y = 1},
    // L // magenta
    {.type = TETROMINO_L,
     .shape = {{0, 0, 3, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     .pivot_x = 1,
     .pivot_y = 1},
    // T // magenta
    {.type = TETROMINO_T,
     .shape = {{0, 3, 0, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     .pivot_x = 1,
     .pivot_y = 1},
    // O // yellow
    {.type = TETROMINO_O,
     .shape = {{0, 4, 4, 0}, {0, 4, 4, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     .pivot_x = 1,
     .pivot_y = 1},
    // S // green
    {.type = TETROMINO_S,
     .shape = {{0, 5, 5, 0}, {5, 5, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     .pivot_x = 1,
     .pivot_y = 1},
    // Z // red
    {.type = TETROMINO_Z,
     .shape = {{6, 6, 0, 0}, {0, 6, 6, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     .pivot_x = 1,
     .pivot_y = 1}};

// ====================================
// Internal funcs for tetromino control

static void moveTetrominoLeft() { the_piece.x--; }

static void moveTetrominoRight() { the_piece.x++; }

static void moveTetrominoDown() { the_piece.y++; }

/**
 * @brief Rotate a tetromino matrix 90 degrees clockwise around its pivot.
 *
 * Uses floating-point pivot coordinates and rounds the result to the nearest
 * integer grid cell.
 *
 * @param matrix 4x4 tetromino matrix to rotate in-place
 */

static void rotateTetromino(int matrix[TETROMINO_SIZE][TETROMINO_SIZE]) {
  float cx = the_piece.base.pivot_x;
  float cy = the_piece.base.pivot_y;
  int temp[TETROMINO_SIZE][TETROMINO_SIZE] = {0};

  for (int y = 0; y < TETROMINO_SIZE; y++) {
    for (int x = 0; x < TETROMINO_SIZE; x++) {
      int v = matrix[y][x];
      if (v == 0) continue;

      // Offset relative to the pivot
      float rel_x = x - cx;
      float rel_y = y - cy;

      // 90-degree clockwise rotation: (x, y) → (y, -x)
      float rot_x = rel_y;
      float rot_y = -rel_x;

      // Transform back to matrix coordinates
      float new_x = rot_x + cx;
      float new_y = rot_y + cy;

      // Round to nearest integer
      int nx = (int)roundf(new_x);
      int ny = (int)roundf(new_y);

      if (nx >= 0 && nx < 4 && ny >= 0 && ny < 4) {
        temp[ny][nx] = v;
      }
    }
  }
  memcpy(matrix, temp, sizeof(temp));
}

/**
 * @brief Get current active tetromino state.
 *
 * @return Active tetromino (position + shape)
 */

ActiveTetromino_t getCurrentTetrominoInfo() { return the_piece; }

/**
 * @brief Detect tetromino type by its shape matrix.
 *
 * @param shape 4x4 matrix
 * @return Corresponding tetromino type
 */

TetrominoType_t detectTetromino(
    const int shape[TETROMINO_SIZE][TETROMINO_SIZE]) {
  TetrominoType_t res = TETROMINO_I;  // just for initializing

  for (int t = 0; t < TETROMINO_COUNT; t++) {
    bool same = TRUE;

    for (int y = 0; y < TETROMINO_SIZE && same; y++) {
      for (int x = 0; x < TETROMINO_SIZE; x++) {
        if (tetrominoes[t].shape[y][x] != shape[y][x]) {
          same = FALSE;
          x = TETROMINO_SIZE - 1;
        }
      }
    }

    if (same) res = tetrominoes[t].type;
  }
  return res;
}

/**
 * @brief Generate a random tetromino shape.
 *
 * @param dest Output 4x4 matrix
 */

void getRandomTetromino(int dest[TETROMINO_SIZE][TETROMINO_SIZE]) {
  int index = rand() % TETROMINO_COUNT;
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    memcpy(dest[y], tetrominoes[index].shape[y], TETROMINO_SIZE * sizeof(int));
  }
}

/**
 * @brief Initialize a new active tetromino from the "next" shape.
 *
 * @param next 4x4 matrix of next tetromino
 */

void spawnTetromino(int next[TETROMINO_SIZE][TETROMINO_SIZE]) {
  TetrominoType_t the_type = detectTetromino(next);

  the_piece.base.type = the_type;
  the_piece.base.pivot_x = tetrominoes[the_type].pivot_x;
  the_piece.base.pivot_y = tetrominoes[the_type].pivot_y;

  for (int y = 0; y < TETROMINO_SIZE; y++) {
    memcpy(the_piece.base.shape[y], next[y], sizeof(next[y]));
  }

  the_piece.x = TETROMINO_SPAWN_X;
  the_piece.y = TETROMINO_SPAWN_Y;
}

/**
 * @brief Handle movement and rotation of the active tetromino.
 *
 * Applies collision checks before modifying the position or rotation.
 *
 * @param action User action
 * @return Updated tetromino state
 */

ActiveTetromino_t tetrominoMovingManager(UserAction_t action) {
  if (!checkMerge()) {
    switch (action) {
      case ACTION_LEFT:
        if (canMoveLeft(the_piece.base.shape, the_piece.x,
                        the_piece.y)) {  // field_control module
          moveTetrominoLeft();
        }
        break;
      case ACTION_RIGHT:
        if (canMoveRight(the_piece.base.shape, the_piece.x,
                         the_piece.y)) {  // field_control module
          moveTetrominoRight();
        }
        break;
      case ACTION_DOWN:
        dropTetromino(the_piece.base.shape, the_piece.x,
                      the_piece.y);  // field_control module
        break;
      case ACTION_ACTION:  // rotate module
        if (the_piece.base.type != TETROMINO_O) {
          int temp[TETROMINO_SIZE][TETROMINO_SIZE];
          for (int y = 0; y < TETROMINO_SIZE; y++) {
            memcpy(temp[y], the_piece.base.shape[y], sizeof(temp[y]));
          }
          rotateTetromino(temp);
          if (canPlace(temp, the_piece.x,
                       the_piece.y)) {  // field_control module
            for (int y = 0; y < TETROMINO_SIZE; y++) {
              memcpy(the_piece.base.shape[y], temp[y], sizeof(temp[y]));
            }
          }
        }
        break;
      case ACTION_NONE:
        if (canMoveDown(the_piece.base.shape, the_piece.x,
                        the_piece.y)) {  // field_control module
          moveTetrominoDown();
        }
        break;

      case ACTION_START:
      case ACTION_PAUSE:
      case ACTION_TERMINATE:
        break;
    }
  }
  return the_piece;
}