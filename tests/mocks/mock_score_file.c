#include "../../../src/core/include/score_file.h"
#include "../include/test_helpers.h"

// =========
// Variables

static int mock_readHighScore_value = 0;
static int mock_score_file_write_calls = 0;

// =======
// Helpers

void mock_score_file_reset() {
  mock_readHighScore_value = 0;
  mock_score_file_write_calls = 0;
}

void mock_set_readHighScore(int v) { mock_readHighScore_value = v; }

int mock_get_score_file_write_calls() { return mock_score_file_write_calls; }

// =====
// Mocks

int readHighScore() { return mock_readHighScore_value; }

bool writeScoreIntoFile(GameInfo_t info) {
  (void)info;
  mock_score_file_write_calls++;
  return TRUE;
}