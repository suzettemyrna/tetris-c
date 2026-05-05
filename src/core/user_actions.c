/**
 * @file user_actions.c
 * @brief Processing and interpretation of user input.
 *
 * This module converts raw key input into high-level actions
 * used by the backend state machine. It also handles player
 * name input during the start state.
 */

/*
  =====================
  Processing user input
  =====================
*/

#include "include/user_actions.h"

#include "include/game_info.h"
#include "include/state_machine.h"

// =============================================================
// Internal buffer for the currently entered player name

static char curr_player_name[MAX_NAME_LENGTH + 1];

// =============================================================
// Internal helpers for player name input

/**
 * @brief Checks whether the current player name is valid.
 *
 * A name is considered valid if it contains at least one
 * non-space character.
 */
static bool playerNameValid() {
  bool res = FALSE;
  for (int i = 0; i < MAX_NAME_LENGTH; i++) {
    if (curr_player_name[i] != ' ') res = TRUE;
  }
  return res;
}

/**
 * @brief Processes key input for player name editing.
 *
 * Supports:
 * - Alphanumeric character input
 * - Backspace for deletion
 */
static void handlePlayerNameInput(int key) {
  if (isalnum(key)) {
    // Replace the first available space with the new character
    for (int i = 0; i < MAX_NAME_LENGTH; i++) {
      if (curr_player_name[i] == ' ') {
        curr_player_name[i] = (char)key;
        i = MAX_NAME_LENGTH;
      }
    }
  }

  if (key == KEY_BACKSPACE || key == 127) {
    // Remove the last non-space character
    for (int i = MAX_NAME_LENGTH - 1; i >= 0; i--) {
      if (curr_player_name[i] != ' ') {
        curr_player_name[i] = ' ';
        i = -1;
      }
    }
  }

  updateGameInfoPlayerName(curr_player_name);  // game_info
}

// =============================================================
// Internal mapping: key -> action

/**
 * @brief Maps raw key input to a backend action.
 *
 * @param key Input key code
 * @return Corresponding UserAction_t value
 */
static UserAction_t mapKeyToAction(int key) {
  UserAction_t res = ACTION_NONE;

  switch (key) {
    case ENTER_KEY:
      if (playerNameValid()) {
        res = ACTION_START;
      }
      break;
    case ' ':
      res = ACTION_PAUSE;
      break;
    case KEY_LEFT:
      res = ACTION_LEFT;
      break;
    case KEY_RIGHT:
      res = ACTION_RIGHT;
      break;
    case KEY_DOWN:
      res = ACTION_DOWN;
      break;
    case KEY_UP:
      res = ACTION_ACTION;
      break;
    case ESCAPE:
      res = ACTION_TERMINATE;
      break;
    default:
      res = ACTION_NONE;
      break;
  }

  return res;
}

// =============================================================
// Public interface

/**
 * @brief Processes input key and returns corresponding action.
 *
 * If the game is in STATE_START, the function also updates
 * the player name buffer.
 *
 * @param key Input key code
 * @return Interpreted user action
 */
UserAction_t handleInput(int key) {
  GameState_t state = getCurrentState();  // state_machine

  if (state == STATE_START) {
    handlePlayerNameInput(key);
  }

  UserAction_t action = mapKeyToAction(key);

  return action;
}

/**
 * @brief Initializes the player name buffer.
 *
 * Fills it with spaces and ensures null-termination.
 */
void initCurrPlayerName() {
  memset(curr_player_name, ' ', MAX_NAME_LENGTH);
  curr_player_name[MAX_NAME_LENGTH] = '\0';
}