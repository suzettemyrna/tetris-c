#include "../../src/cli/include/cli.h"
#include "../include/test_helpers.h"

// ==============================
// Variables and internal objects

int mock_cli_init_calls = 0;
int mock_cli_end_calls = 0;
int mock_drawingManager_calls = 0;

static int mock_input_seq[128];
static int mock_input_len = 0;
static int mock_input_pos = 0;

static int mock_iteration_limit = 1;
static int mock_iteration_count = 0;

static ControlFlags_t mock_control_flags = {
    .first_iteration = 0, .game_finished = 0, .exit_required = 0};

// =======
// Helpers

void mock_cli_reset() {
  mock_cli_init_calls = 0;
  mock_cli_end_calls = 0;
  mock_drawingManager_calls = 0;
  mock_input_len = 0;
  mock_input_pos = 0;
  mock_iteration_limit = 1;
  mock_iteration_count = 0;
  memset(&mock_control_flags, 0, sizeof(mock_control_flags));
}

void mock_set_iteration_limit(int n) {
  if (n < 1) n = 1;
  mock_iteration_limit = n;
}

void mock_set_input_sequence(const int *seq, int len) {
  if (len > (int)(sizeof(mock_input_seq) / sizeof(int)))
    len = sizeof(mock_input_seq) / sizeof(int);
  for (int i = 0; i < len; ++i) mock_input_seq[i] = seq[i];
  mock_input_len = len;
  mock_input_pos = 0;
}

// =====
// Mocks

void cliInit() { mock_cli_init_calls++; }

void cliEnd() { mock_cli_end_calls++; }

void drawingManager() {
  mock_drawingManager_calls++;
  mock_iteration_count++;
  if (mock_iteration_count >= mock_iteration_limit) {
    mock_control_flags.exit_required = 1;
  }
}

UserInput_t getInput() {
  UserInput_t r;
  if (mock_input_pos < mock_input_len) {
    r.key = mock_input_seq[mock_input_pos++];
    r.hold = 0;
  } else {
    r.key = NO_INPUT;
    r.hold = 0;
  }
  return r;
}

ControlFlags_t getGameControlFlags() { return mock_control_flags; }