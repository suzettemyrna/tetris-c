#include "../../../src/core/include/state_machine.h"
#include "../include/test_helpers.h"

// =========================
// Internal object for tests

static StateMachine mock_current = {.state = STATE_START};

// ======
// Helper

void setState(GameState_t new_state) { mock_current.state = new_state; }

// =====
// Mocks

GameState_t getCurrentState() { return mock_current.state; }