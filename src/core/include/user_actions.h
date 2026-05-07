#ifndef USER_ACTIONS_H
#define USER_ACTIONS_H

/**
 * @file user_actions.h
 * @brief User input processing interface.
 *
 * This module provides functions to convert raw input into
 * backend actions and to manage player name input.
 */

#include <ctype.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "../../game_model/game_model.h"
#include "../../shared/game_config.h"
#include "core_context.h"

/**
 * @brief Processes a key input and converts it to a user action.
 *
 * This function is the main entry point for input handling.
 * It also updates player name input when in the start state.
 *
 * @param key Input key code (ncurses)
 * @return Corresponding UserAction_t value
 */
UserAction_t handleInput(int key);

/**
 * @brief Initializes internal player name buffer.
 *
 * Must be called before starting a new game session.
 */
void initCurrPlayerName(void);

#endif  // USER_ACTIONS_H