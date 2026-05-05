#include "../../../src/core/include/field_control.h"
#include "../include/test_helpers.h"

// =========
// Variables

static bool mock_canMoveLeft_val = 0;
static bool mock_canMoveRight_val = 0;
static bool mock_canMoveDown_val = 0;
static bool mock_canPlace_val = 1;
static int mock_checkMerge_val = 0;
static int mock_drop_called = 0;
static int mock_updateFieldWithActive_called = 0;

// =======
// Helpers

void mock_field_control_reset() {
    mock_canMoveLeft_val = 0;
    mock_canMoveRight_val = 0;
    mock_canMoveDown_val = 0;
    mock_canPlace_val = 1;
    mock_checkMerge_val = 0;
    mock_drop_called = 0;
    mock_updateFieldWithActive_called = 0;
}

void mock_set_canMoveLeft(int v) { mock_canMoveLeft_val = v; }
void mock_set_canMoveRight(int v) { mock_canMoveRight_val = v; }
void mock_set_canMoveDown(int v) { mock_canMoveDown_val = v; }
void mock_set_canPlace(int v) { mock_canPlace_val = v; }
void mock_set_checkMerge(int v) { mock_checkMerge_val = v; }
void mock_set_drop_called_count(int v) { mock_drop_called = v; }

int mock_get_drop_called() { return mock_drop_called; }
int mock_get_updateFieldWithActive_called() { return mock_updateFieldWithActive_called; }

// =====
// Mocks

bool canMoveLeft(int shape[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
    (void)shape; (void)x; (void)y;
    return mock_canMoveLeft_val;
}

bool canMoveRight(int shape[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
    (void)shape; (void)x; (void)y;
    return mock_canMoveRight_val;
}

bool canMoveDown(int shape[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
    (void)shape; (void)x; (void)y;
    return mock_canMoveDown_val;
}

bool canPlace(int shape[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
    (void)x; (void)y;
    (void)shape;
    return mock_canPlace_val;
}

void dropTetromino(int shape[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
    (void)shape; (void)x; (void)y;
    mock_drop_called++;
}

void updateFieldWithActiveTetromino(int shape[TETROMINO_SIZE][TETROMINO_SIZE], int x, int y) {
    (void)shape; (void)x; (void)y;
    mock_updateFieldWithActive_called++;
}

bool checkMerge() {
    return mock_checkMerge_val ? TRUE : FALSE;
}