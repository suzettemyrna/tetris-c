#ifndef BACKEND_OBJECTS_H
#define BACKEND_OBJECTS_H

/**
 * @file backend_objects.h
 * @brief Core data structures used in the Tetris backend.
 *
 * This file defines internal structures representing the game state,
 * tetrominoes, and control flow flags.
 */

#include <stdbool.h>

#include "../../game_model/game_model.h"
#include "backend_defines.h"

/**
 * @brief Flags controlling global game execution flow.
 */
typedef struct {
  bool first_iteration; /**< Indicates first loop iteration */
  bool game_finished;   /**< Indicates that game over has been processed */
  bool exit_required;   /**< Indicates that application should terminate */
} ControlFlags_t;

/**
 * @brief Enumeration of user actions interpreted by the backend.
 */
typedef enum {
  ACTION_START,     /**< Start game (Enter key) */
  ACTION_PAUSE,     /**< Pause game (Space key) */
  ACTION_TERMINATE, /**< Exit game (Escape key) */
  ACTION_LEFT,      /**< Move tetromino left */
  ACTION_RIGHT,     /**< Move tetromino right */
  ACTION_DOWN,      /**< Drop tetromino instantly */
  ACTION_ACTION,    /**< Rotate tetromino (Up key) */
  ACTION_NONE       /**< No action */
} UserAction_t;

/**
 * @brief Enumeration of all tetromino types.
 */
typedef enum {
  TETROMINO_I,
  TETROMINO_J,
  TETROMINO_L,
  TETROMINO_T,
  TETROMINO_O,
  TETROMINO_S,
  TETROMINO_Z
} TetrominoType_t;

/**
 * @brief Static definition of a tetromino shape.
 *
 * Represents a 4x4 matrix and rotation pivot.
 */
typedef struct {
  TetrominoType_t type;                      /**< Type of tetromino */
  int shape[TETROMINO_SIZE][TETROMINO_SIZE]; /**< Shape matrix */
  float pivot_x;                             /**< Rotation pivot X coordinate */
  float pivot_y;                             /**< Rotation pivot Y coordinate */
} Tetromino_t;

/**
 * @brief Active tetromino instance on the field.
 */
typedef struct {
  Tetromino_t base; /**< Base tetromino definition */
  int x;            /**< Current X position on field */
  int y;            /**< Current Y position on field */
} ActiveTetromino_t;

/**
 * @brief Game field representation.
 */
typedef struct {
  int field[FIELD_HEIGHT][FIELD_WIDTH]; /**< Field grid */
} Field_t;

#endif  // BACKEND_OBJECTS_H