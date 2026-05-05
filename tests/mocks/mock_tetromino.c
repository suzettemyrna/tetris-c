#include "../../../src/core/include/tetromino.h"
#include "../include/test_helpers.h"

// =========
// Variables

static int mock_random_piece[TETROMINO_SIZE][TETROMINO_SIZE];
static ActiveTetromino_t mock_active_piece;
static int mock_spawn_called = 0;

// =======
// Helpers

void mock_tetromino_reset() {
  memset(mock_random_piece, 0, sizeof(mock_random_piece));
  memset(&mock_active_piece, 0, sizeof(mock_active_piece));
  mock_spawn_called = 0;
}

void mock_set_random_piece(const int piece[TETROMINO_SIZE][TETROMINO_SIZE]) {
  for (int y = 0; y < TETROMINO_SIZE; ++y)
    for (int x = 0; x < TETROMINO_SIZE; ++x)
      mock_random_piece[y][x] = piece[y][x];
}

void mock_set_active_piece(ActiveTetromino_t p) { mock_active_piece = p; }

int mock_get_spawn_called(void) { return mock_spawn_called; }

// =====
// Mocks

ActiveTetromino_t tetrominoMovingManager(UserAction_t action) {
  (void)action;
  return mock_active_piece;
}

void getRandomTetromino(int out[TETROMINO_SIZE][TETROMINO_SIZE]) {
  for (int y = 0; y < TETROMINO_SIZE; y++)
    for (int x = 0; x < TETROMINO_SIZE; x++)
      out[y][x] = mock_random_piece[y][x];
}

void spawnTetromino(int shape[TETROMINO_SIZE][TETROMINO_SIZE]) {
  (void)shape;
  mock_spawn_called++;
}

ActiveTetromino_t getCurrentTetrominoInfo() { return mock_active_piece; }