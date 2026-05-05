/*
# ===========================================================
  Connecting the interface and the internal state of the game
# ===========================================================
*/

#include "include/tetris_api.h"

#include "../cli/include/cli.h"
#include "../core/include/game_info.h"
#include "../core/include/state_machine.h"
#include "../core/include/user_actions.h"

// =======================
// Internal timing utility

static long long getTimeMs() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// ===============
// Main game cycle

void tetrisGameLoop() {
  cliInit();             // cli
  initCurrPlayerName();  // user_actions module
  initGameInfo();        // game_info module

  long long last_fall_time = getTimeMs();
  while (!getGameControlFlags().exit_required) {  // state_machine module
    if (isFirstIteration()) {                     // state_machine module
      resetGameInfo();                            // game_info module
    }

    UserInput_t input = getInput();  // cli
    UserAction_t action = ACTION_NONE;

    if (input.key != NO_INPUT) {
      action = handleInput(input.key);  // user_actions module
    }

    GameState_t state = getCurrentState();   // state_machine module
    GameInfo_t info = getCurrentGameInfo();  // game_info module

    if (state == STATE_MOVE || state == STATE_SHIFT) {
      // Automatic piece falling
      long long current_time = getTimeMs();
      long long elapsed = current_time - last_fall_time;

      if (info.speed > 0 && elapsed >= info.speed) {
        handleStates(ACTION_NONE, false);  // state_machine module
        last_fall_time = current_time;     // Reset fall timer
      }
    }

    if (action != ACTION_NONE || state == STATE_SPAWN || state == STATE_MERGE) {
      handleStates(action, input.hold);
    }
    drawingManager();  // cli
  }

  cliEnd();         // cli
  cleanGameInfo();  // game_info module
}