#ifndef GAME_LOOP_H
#define GAME_LOOP_H

/**
 * @file game_loop.h
 * @brief Connects frontend input/rendering with backend game logic.
 */

/**
 * @brief Runs the main Tetris game loop.
 *
 * This function blocks until the game is terminated.
 * It is responsible for coordinating:
 * - input handling
 * - state updates
 * - rendering
 */
void tetrisGameLoop(void);

#endif  // GAME_LOOP_H