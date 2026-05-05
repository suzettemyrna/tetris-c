#include "../../../src/core/include/field_control.h"
#include "../include/test_helpers.h"

// =========
// Variables

static Field_t mock_current_field;
static int mock_clearCompletedLines_return = 0;
static bool mock_checkGameOver_return = FALSE;
static bool mock_checkMerge_return = FALSE;
static int mock_updateFieldWithActive_called = 0;

// =======
// Helpers

void mock_field_control_reset() {
  memset(&mock_current_field, 0, sizeof(mock_current_field));
  mock_clearCompletedLines_return = 0;
  mock_checkGameOver_return = 0;
  mock_checkMerge_return = 0;
  mock_updateFieldWithActive_called = 0;
}

void mock_set_current_field_value(int v) {
  for (int y = 0; y < FIELD_HEIGHT; ++y)
    for (int x = 0; x < FIELD_WIDTH; ++x) mock_current_field.field[y][x] = v;
}

void mock_set_clearCompletedLines_return(int v) {
  mock_clearCompletedLines_return = v;
}

void mock_set_GI_checkGameOver_return(int v) { mock_checkGameOver_return = v; }

void mock_set_GI_checkMerge_return(int v) { mock_checkMerge_return = v; }

// =====
// Mocks

void fieldInit() {}

Field_t getCurrentFieldWithActiveTetromino() { return mock_current_field; }

void resetMergeStatus() {}

void updateFieldWithActiveTetromino(int shape[TETROMINO_SIZE][TETROMINO_SIZE],
                                    int x, int y) {
  (void)shape;
  (void)x;
  (void)y;
  mock_updateFieldWithActive_called++;
}

int clearCompletedLines() { return mock_clearCompletedLines_return; }

bool checkGameOverCondition(int next[TETROMINO_SIZE][TETROMINO_SIZE]) {
  (void)next;
  return mock_checkGameOver_return;
}

bool checkMerge() { return mock_checkMerge_return; }

int mock_get_updateFieldWithActive_called() {
  return mock_updateFieldWithActive_called;
}