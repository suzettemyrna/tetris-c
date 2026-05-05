#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

/**
 * @file game_config.h
 * @brief Global configuration constants for the Tetris game.
 *
 * This file defines shared constants used across all layers
 * (core, API, and frontend).
 */

/** Width of the game field (in cells) */
#define FIELD_WIDTH 10

/** Height of the game field (in cells) */
#define FIELD_HEIGHT 20

/**
 * @brief Size of the tetromino matrix.
 *
 * All tetrominoes are represented as 4x4 matrices.
 */
#define TETROMINO_SIZE 4

/** Maximum length of player name (excluding null terminator) */
#define MAX_NAME_LENGTH 10

/** Maximum number of digits in score representation */
#define MAX_SCORE_LENGTH 6

/** Value representing absence of input */
#define NO_INPUT -1

#endif  // GAME_CONFIG_H