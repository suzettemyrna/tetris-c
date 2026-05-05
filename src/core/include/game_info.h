/**
 * @file game_info.h
 * @brief Public interface for game state management.
 */

#ifndef GAME_INFO_H
#define GAME_INFO_H

#include <stdlib.h>
#include <time.h>

#include "../../api/include/tetris_api.h"
#include "../../shared/game_config.h"
#include "backend_objects.h"

/**
 * @brief Get current game state snapshot.
 */
GameInfo_t getCurrentGameInfo();

/**
 * @brief Initialize game state.
 */
void initGameInfo();

/**
 * @brief Free all allocated resources.
 */
void cleanGameInfo();

/**
 * @brief Update player name.
 */
void updateGameInfoPlayerName(const char *player_name);

/**
 * @brief Reset game state for a new game.
 */
void resetGameInfo();

/**
 * @brief Update visible field.
 */
void updateField();

/**
 * @brief Spawn new tetromino.
 */
void newTetromino();

/**
 * @brief Move active tetromino.
 */
void moveTetromino(UserAction_t action);

/**
 * @brief Check game over condition.
 */
bool gameOverCondition();

/**
 * @brief Check merge state.
 */
bool isThereMerge();

/**
 * @brief Update score and level.
 */
void updateScore();

/**
 * @brief Save score to file.
 */
void writeDownScore();

#endif  // GAME_INFO_H