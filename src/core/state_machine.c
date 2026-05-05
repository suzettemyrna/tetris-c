/**
 * @file state_machine.c
 * @brief Finite State Machine (FSM) logic for the Tetris game.
 *
 * This module controls the main game flow using a finite state machine.
 * It processes user actions and updates the current game state accordingly.
 */

/*
  ================================
  Finite State Machine (FSM) logic
  ================================
*/

#include "include/state_machine.h"

#include "include/game_info.h"

// =================
// Internal objects

/// Current state of the FSM
static StateMachine current = {.state = STATE_START};

/// Global control flags
static ControlFlags_t flags = {
    .first_iteration = TRUE, .game_finished = FALSE, .exit_required = FALSE};

// =================
// Internal helpers

/// Initialize a new game session
static void newGame() {
  flags.first_iteration = TRUE;
  flags.game_finished = FALSE;
}

/// Mark the game as finished
static void finishGame() { flags.game_finished = TRUE; }

/// Request game exit
static void exitGame() { flags.exit_required = TRUE; }

// ======================================
// State handling functions

/// Handle STATE_START
static void handleStartState(UserAction_t action) {
  // Possible transitions:
  // - STATE_SPAWN
  // - exit game

  if (action == ACTION_START) {
    newGame();
    current.state = STATE_SPAWN;

  } else if (action == ACTION_TERMINATE) {
    exitGame();
  }
}

/// Handle STATE_SPAWN
static void handleSpawnState() {
  // Possible transitions:
  // - STATE_MOVE
  // Pause is not allowed here

  newTetromino();
  updateField();

  current.state = STATE_MOVE;
}

/// Handle STATE_MOVE
static void handleMoveState(UserAction_t action, bool hold) {
  // Possible transitions:
  // - STATE_SHIFT
  // - STATE_PAUSE
  // - STATE_MERGE
  // - STATE_GAME_OVER

  if (action == ACTION_PAUSE) {
    current.state = STATE_PAUSE;

  } else if (action == ACTION_TERMINATE) {
    current.state = STATE_GAME_OVER;

  } else {
    bool merged = isThereMerge();
    if (!merged) {
      moveTetromino(action);
      updateField();
      merged = isThereMerge();
    }

    if (hold && (action == ACTION_LEFT || action == ACTION_RIGHT)) {
      current.state = STATE_SHIFT;

    } else if (merged) {
      current.state = STATE_MERGE;
    }
  }
}

/// Handle STATE_SHIFT
static void handleShiftState(UserAction_t action, bool hold) {
  // Pause and terminate are not allowed here

  if (!isThereMerge()) {
    moveTetromino(action);
    updateField();
  }

  // Exit SHIFT when the key is released
  if (!hold || !(action == ACTION_LEFT || action == ACTION_RIGHT)) {
    current.state = STATE_MOVE;

  } else if (isThereMerge()) {
    current.state = STATE_MERGE;
  }
}

/// Handle STATE_MERGE
static void handleMergeState() {
  // Possible transitions:
  // - STATE_SPAWN
  // - STATE_GAME_OVER
  // Pause and terminate are not allowed here

  if (gameOverCondition()) {
    current.state = STATE_GAME_OVER;

  } else {
    current.state = STATE_SPAWN;
    updateField();
    updateScore();
  }
}

/// Handle STATE_PAUSE
static void handlePauseState(UserAction_t action) {
  // Possible transitions:
  // - STATE_MOVE
  // - STATE_GAME_OVER

  if (action == ACTION_PAUSE)
    current.state = STATE_MOVE;

  else if (action == ACTION_TERMINATE)
    current.state = STATE_GAME_OVER;
}

/// Handle STATE_GAME_OVER
static void handleGameOverState(UserAction_t action) {
  // Possible transitions:
  // - STATE_START
  // - exit game

  if (!flags.game_finished) {
    writeDownScore();
    finishGame();
  }

  if (action == ACTION_START) {
    current.state = STATE_START;

  } else if (action == ACTION_TERMINATE) {
    exitGame();
  }
}

// ==================================
// Shared helper

/**
 * @brief Check if this is the first iteration of the game loop.
 *
 * @return true if first iteration, false otherwise
 */
bool isFirstIteration() {
  bool res = flags.first_iteration;
  if (res) {
    flags.first_iteration = FALSE;
  }
  return res;
}

// =================
// Read-only API

/**
 * @brief Get current game state.
 *
 * @return Current FSM state
 */
GameState_t getCurrentState() { return current.state; }

/**
 * @brief Get control flags.
 *
 * @return Current control flags structure
 */
ControlFlags_t getGameControlFlags() { return flags; }

// =================
// FSM entry point

/**
 * @brief Process one FSM step.
 *
 * @param action User action
 * @param hold Whether the key is being held
 */
void handleStates(UserAction_t action, bool hold) {
  switch (current.state) {
    case STATE_START:
      handleStartState(action);
      break;
    case STATE_SPAWN:
      handleSpawnState();
      break;
    case STATE_MOVE:
      handleMoveState(action, hold);
      break;
    case STATE_SHIFT:
      handleShiftState(action, hold);
      break;
    case STATE_MERGE:
      handleMergeState();
      break;
    case STATE_PAUSE:
      handlePauseState(action);
      break;
    case STATE_GAME_OVER:
      handleGameOverState(action);
      break;
  }
}