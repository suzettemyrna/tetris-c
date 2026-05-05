/**
 * @file game_info.c
 * @brief Game state storage, initialization, and update logic.
 *
 * This module owns the main GameInfo_t structure and is responsible for:
 * - memory allocation and cleanup
 * - synchronization with field and tetromino subsystems
 * - score, level, and speed management
 */

/*
  ====================================================
  Initializing, storing, and updating game information
  ====================================================
*/

#include "include/game_info.h"

#include "include/field_control.h"
#include "include/score_file.h"
#include "include/tetromino.h"

// =================
// Internal object

/// Global game state
static GameInfo_t game_info;

// =================
// Internal helpers

/// Generate and store the next tetromino
static void updateNextTetromino() {
  int random_shape[TETROMINO_SIZE][TETROMINO_SIZE];
  getRandomTetromino(random_shape);  // tetromino module

  for (int y = 0; y < TETROMINO_SIZE; y++) {
    for (int x = 0; x < TETROMINO_SIZE; x++) {
      game_info.next[y][x] = random_shape[y][x];
    }
  }
}

/// Update level and corresponding speed
static void updateLevelAndSpeed() {
  if (game_info.level < MAX_LEVEL) game_info.level += 1;
  game_info.speed = GET_TIMEOUT_VALUE(game_info.level);
}

// ====================================
// Read-only API (used by other modules)

/**
 * @brief Get current game state snapshot.
 *
 * @return Copy of GameInfo_t. Returns empty struct if not initialized.
 */
GameInfo_t getCurrentGameInfo() {
  GameInfo_t res = game_info;
  if (game_info.field == NULL || game_info.next == NULL) {
    GameInfo_t empty_info = {0};
    res = empty_info;
  }
  return res;
}

// ====================================
// Initialization / cleanup (API layer)

/**
 * @brief Allocate and initialize game state.
 *
 * Must be called before the game loop starts.
 */
void initGameInfo() {
  srand(time(NULL));  // initialize RNG

  // 1. Game field allocation
  if (game_info.field == NULL) {
    game_info.field = calloc(FIELD_HEIGHT, sizeof(int *));
    for (int y = 0; y < FIELD_HEIGHT; y++) {
      game_info.field[y] = calloc(FIELD_WIDTH, sizeof(int));
    }
  }

  // 2. Next tetromino buffer
  if (game_info.next == NULL) {
    game_info.next = calloc(TETROMINO_SIZE, sizeof(int *));
    for (int y = 0; y < TETROMINO_SIZE; y++) {
      game_info.next[y] = calloc(TETROMINO_SIZE, sizeof(int));
    }
  }

  // 3. Player name buffer
  if (game_info.player == NULL) {
    game_info.player = calloc(MAX_NAME_LENGTH + 1, sizeof(char));
    memset(game_info.player, ' ', MAX_NAME_LENGTH);
    game_info.player[MAX_NAME_LENGTH] = '\0';
  }

  // 4. Initialize field subsystem
  fieldInit();

  // 5. Initialize gameplay values
  game_info.score = MIN_POINTS;
  game_info.level = MIN_LEVEL;
  game_info.speed = GET_TIMEOUT_VALUE(game_info.level);

  // 6. Load high score
  int hs = readHighScore();
  game_info.high_score = hs > 0 ? hs : 0;

  // 7. Generate first next tetromino
  int piece[TETROMINO_SIZE][TETROMINO_SIZE];
  getRandomTetromino(piece);
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    memcpy(game_info.next[y], piece[y], TETROMINO_SIZE * sizeof(int));
  }
}

/**
 * @brief Free all allocated memory in game_info.
 */
void cleanGameInfo() {
  if (game_info.field) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      free(game_info.field[i]);
      game_info.field[i] = NULL;
    }
    free(game_info.field);
    game_info.field = NULL;
  }

  if (game_info.next) {
    for (int i = 0; i < TETROMINO_SIZE; i++) {
      free(game_info.next[i]);
      game_info.next[i] = NULL;
    }
    free(game_info.next);
    game_info.next = NULL;
  }

  if (game_info.player) {
    free(game_info.player);
    game_info.player = NULL;
  }
}

// ====================================
// State-machine-driven updates

/**
 * @brief Reset game state for a new session.
 */
void resetGameInfo() {
  // 1. Clear field
  if (game_info.field) {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
      memset(game_info.field[y], 0, FIELD_WIDTH * sizeof(int));
    }
  }

  // 2. Clear next tetromino buffer
  if (game_info.next) {
    for (int y = 0; y < TETROMINO_SIZE; y++) {
      memset(game_info.next[y], 0, TETROMINO_SIZE * sizeof(int));
    }
  }

  // 3. Generate new next tetromino
  int piece[TETROMINO_SIZE][TETROMINO_SIZE];
  getRandomTetromino(piece);
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    memcpy(game_info.next[y], piece[y], TETROMINO_SIZE * sizeof(int));
  }

  // 4. Reset score and level
  game_info.score = MIN_POINTS;
  game_info.level = MIN_LEVEL;
  game_info.speed = GET_TIMEOUT_VALUE(game_info.level);

  // 5. Reload high score
  int hs = readHighScore();
  game_info.high_score = hs > 0 ? hs : 0;

  // Player name is intentionally preserved

  // 6. Reinitialize field subsystem
  fieldInit();
}

/**
 * @brief Synchronize visible field with backend field state.
 */
void updateField() {
  Field_t current = getCurrentFieldWithActiveTetromino();  // field_control
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    memcpy(game_info.field[y], current.field[y], FIELD_WIDTH * sizeof(int));
  }
}

/**
 * @brief Spawn new active tetromino and update state.
 */
void newTetromino() {
  int arg[TETROMINO_SIZE][TETROMINO_SIZE];
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    memcpy(arg[y], game_info.next[y], sizeof(arg[y]));
  }

  spawnTetromino(arg);
  ActiveTetromino_t curr_piece = getCurrentTetrominoInfo();

  resetMergeStatus();

  updateFieldWithActiveTetromino(curr_piece.base.shape, curr_piece.x,
                                 curr_piece.y);

  updateNextTetromino();
}

/**
 * @brief Move active tetromino based on user action.
 */
void moveTetromino(UserAction_t action) {
  ActiveTetromino_t curr_tetromino = tetrominoMovingManager(action);
  updateFieldWithActiveTetromino(curr_tetromino.base.shape, curr_tetromino.x,
                                 curr_tetromino.y);
}

/**
 * @brief Check game over condition.
 *
 * @return true if game is over
 */
bool gameOverCondition() {
  int arg[TETROMINO_SIZE][TETROMINO_SIZE];
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    memcpy(arg[y], game_info.next[y], sizeof(arg[y]));
  }
  return checkGameOverCondition(arg);
}

/**
 * @brief Check if current tetromino has merged.
 */
bool isThereMerge() { return checkMerge(); }

/**
 * @brief Update score, level, and high score.
 */
void updateScore() {
  int lines_cleared = clearCompletedLines();
  int plus_score = 0;
  int old_score = game_info.score;

  switch (lines_cleared) {
    case 1:
      plus_score = 100;
      break;
    case 2:
      plus_score = 300;
      break;
    case 3:
      plus_score = 700;
      break;
    case 4:
      plus_score = 1500;
      break;
    default:
      plus_score = 0;
  }

  game_info.score += plus_score;

  // Level increases every 600 points
  int old_level_threshold = old_score / 600;
  int new_level_threshold = game_info.score / 600;

  int level_increase = new_level_threshold - old_level_threshold;
  if (level_increase > 0) {
    for (int i = 0; i < level_increase; i++) {
      updateLevelAndSpeed();
    }
  }

  if (game_info.score > game_info.high_score) {
    game_info.high_score = game_info.score;
  }
}

/**
 * @brief Save score to persistent storage.
 */
void writeDownScore() {
  if (game_info.score > 0) {
    writeScoreIntoFile(game_info);
  }
}

// ====================================
// User input interaction

/**
 * @brief Update player name.
 *
 * @param player_name New player name buffer
 */
void updateGameInfoPlayerName(const char *player_name) {
  if (player_name == NULL || game_info.player == NULL) {
    return;
  }

  memcpy(game_info.player, player_name, MAX_NAME_LENGTH + 1);
}