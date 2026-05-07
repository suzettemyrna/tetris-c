#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

/**
 * @file core_config.h
 * @brief Internal constants for Tetris backend logic.
 *
 * This file contains implementation-specific constants used by the backend.
 * These values are not part of the public API and may change.
 */

#undef TRUE
#define TRUE 1

#undef FALSE
#define FALSE 0

/** Minimum possible score */
#define MIN_POINTS 0

/** Maximum possible score */
#define MAX_POINTS 999999

/** Minimum game level */
#define MIN_LEVEL 1

/** Maximum game level */
#define MAX_LEVEL 10

/** Initial timeout value (slowest speed, lowest level) */
#define MAX_TIMEOUT_VALUE 500

/** Decrease in timeout per level (speed increase step) */
#define TIMEOUT_VALUE_STEP 50

/** Minimum timeout value (fastest speed, highest level) */
#define MIN_TIMEOUT_VALUE 50

/**
 * @brief Calculates timeout value based on current level.
 *
 * The timeout decreases as the level increases, down to a minimum limit.
 */
#define GET_TIMEOUT_VALUE(level)                                  \
  (((MAX_TIMEOUT_VALUE - ((level) - 1) * TIMEOUT_VALUE_STEP) >    \
    MIN_TIMEOUT_VALUE)                                            \
       ? (MAX_TIMEOUT_VALUE - ((level) - 1) * TIMEOUT_VALUE_STEP) \
       : MIN_TIMEOUT_VALUE)

/** Key code for Escape */
#define ESCAPE 27

/** Key code for Enter */
#define ENTER_KEY 10

/** Total number of tetromino types */
#define TETROMINO_COUNT 7

/** Initial X position for spawning tetromino */
#define TETROMINO_SPAWN_X 3

/** Initial Y position for spawning tetromino */
#define TETROMINO_SPAWN_Y 0

#endif  // CORE_CONFIG_H