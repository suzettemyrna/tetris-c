#include "../../src/core/include/user_actions.h"
#include "../include/test_helpers.h"

// =========
// Variables

int mock_initCurrPlayerName_calls = 0;
UserAction_t mock_handleInput_return = ACTION_NONE;

// =====
// Mocks

void initCurrPlayerName() { mock_initCurrPlayerName_calls++; }

UserAction_t handleInput(int key) {
  (void)key;
  return mock_handleInput_return;
}