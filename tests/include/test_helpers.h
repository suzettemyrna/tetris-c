#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../src/game_model/game_model.h"
#include "../../src/shared/game_config.h"
#include "../../src/core/include/core_context.h"

/**
 * @file test_helpers.h
 * @brief Mock and helper interfaces for unit testing.
 *
 * Provides control over CLI, state machine, game logic, and I/O layers.
 */

/* =========================
 * CLI mock
 * ========================= */

/** Set a single simulated user input key */
void setUserInput(int set_key);

/** Reset CLI mock state */
void mock_cli_reset();

/** Limit number of main loop iterations */
void mock_set_iteration_limit(int n);

/** Provide predefined input sequence */
void mock_set_input_sequence(const int *seq, int len);

/* =========================
 * Field control mock (game info layer)
 * ========================= */

void mock_field_control_reset();
void mock_set_clearCompletedLines_return(int v);
void mock_set_GI_checkGameOver_return(int v);
void mock_set_GI_checkMerge_return(int v);
void mock_set_current_field_value(int fill_value);

int mock_get_updateFieldWithActive_called();

/* =========================
 * Tetromino movement mock
 * ========================= */

void mock_field_control_reset();
void mock_set_canMoveLeft(int v);
void mock_set_canMoveRight(int v);
void mock_set_canMoveDown(int v);
void mock_set_canPlace(int v);
void mock_set_checkMerge(int v);
void mock_set_drop_called_count(int v);

int mock_get_drop_called();
int mock_get_updateFieldWithActive_called();

/* =========================
 * State machine / game logic mock
 * ========================= */

void mock_set_SM_checkMerge_return(bool v);
void mock_set_SM_checkGameOver_return(bool v);

void mock_state_reset();
void mock_set_state_sequence(const GameState_t *seq, int len);

void setState(GameState_t new_state);

/* =========================
 * Score file mock
 * ========================= */

void mock_score_file_reset();
void mock_set_readHighScore(int v);
int mock_get_score_file_write_calls();

/* =========================
 * Tetromino mock
 * ========================= */

void mock_tetromino_reset();
void mock_set_random_piece(const int piece[TETROMINO_SIZE][TETROMINO_SIZE]);
void mock_set_active_piece(ActiveTetromino_t p);
int mock_get_spawn_called();

#endif  // TEST_HELPERS_H