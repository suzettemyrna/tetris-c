#include "../../../src/core/include/game_info.h"
#include "../include/test_helpers.h"

// =========
// Variables

bool checkMerge_return = TRUE;
bool checkGameOver_return = TRUE;

// =======
// Helpers

void mock_set_SM_checkMerge_return(bool v) { checkMerge_return = v; }

void mock_set_SM_checkGameOver_return(bool v) { checkGameOver_return = v; }

// =====
// Mocks

bool gameOverCondition() { return checkGameOver_return; }

bool isThereMerge() { return checkMerge_return; }

void resetGameInfo() {}

void updateGameInfoPlayerName(const char *player_name) { (void)player_name; }

void updateField() {}

void newTetromino() {}

void moveTetromino(UserAction_t action) { (void)action; }

void updateScore() {}

void writeDownScore() {}