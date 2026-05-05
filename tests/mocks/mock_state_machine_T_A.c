#include "../../src/core/include/state_machine.h"
#include "../include/test_helpers.h"

// ==========================
// Internal objects for tests

static GameState_t mock_state_seq[128];
static int mock_state_len = 0;

// =========
// Variables

static int mock_state_call_index = 0;
static bool mock_first_iteration_flag = true;
int mock_handleStates_calls = 0;

// =======
// Helpers

void mock_state_reset() {
  mock_state_len = 0;
  mock_state_call_index = 0;
  mock_handleStates_calls = 0;
  mock_first_iteration_flag = true;
  memset(mock_state_seq, 0, sizeof(mock_state_seq));
}

void mock_set_state_sequence(const GameState_t *seq, int len) {
  if (len <= 0) {
    mock_state_len = 0;
    mock_state_call_index = 0;
  } else {
    if (len > (int)(sizeof(mock_state_seq) / sizeof(GameState_t))) {
      len = (int)(sizeof(mock_state_seq) / sizeof(GameState_t));
    }
    for (int i = 0; i < len; ++i) mock_state_seq[i] = seq[i];
    mock_state_len = len;
    mock_state_call_index = 0;
  }
}

// =====
// Mocks

GameState_t getCurrentState() {
  GameState_t res = STATE_MOVE;
  if (mock_state_len != 0) {
    int idx = mock_state_call_index;
    if (idx >= mock_state_len) idx = mock_state_len - 1;
    if (mock_state_call_index < mock_state_len) mock_state_call_index++;
    res = mock_state_seq[idx];
  }

  return res;
}

void handleStates(UserAction_t action, bool hold) {
  (void)action;
  (void)hold;
  mock_handleStates_calls++;
}

bool isFirstIteration() {
  bool res = false;
  if (mock_first_iteration_flag) {
    mock_first_iteration_flag = false;
    res = true;
  }
  return res;
}