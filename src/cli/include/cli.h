#ifndef CLI_H
#define CLI_H

#include <ncurses.h>
#include <string.h>

#include "../../api/include/tetris_api.h"

/**
 * @file cli.h
 * @brief Terminal interface rendering and input handling.
 */

#define HOLD_THRESHOLD 2
#define INPUT_POLL_INTERVAL 70

#define PIXEL "[]"
#define VOID "  "
#define CLI_FIELD_WIDTH (FIELD_WIDTH * 2)

/**
 * @brief Reads current user input.
 *
 * @return Structure containing key code and hold state.
 */
UserInput_t getInput();

/**
 * @brief Initializes CLI subsystem.
 */
void cliInit();

/**
 * @brief Shuts down CLI subsystem.
 */
void cliEnd();

/**
 * @brief Draws the current game screen.
 */
void drawingManager();

#endif  // CLI_H