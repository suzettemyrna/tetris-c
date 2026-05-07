#ifndef SCORE_FILE_H
#define SCORE_FILE_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../shared/game_config.h"
#include "core_context.h"

/**
 * @file score_file.h
 * @brief High score file management.
 */

#ifdef UNIT_TEST
#define SCORE_FILE "tests/tests_assets/test_scorefile.txt"
#else
#define SCORE_FILE "assets/scorefile.txt"
#endif

/**
 * @brief Writes the current player's score to the score file.
 *
 * @param game_info Current game information.
 * @return true if writing succeeded, false otherwise.
 */
bool writeScoreIntoFile(GameInfo_t game_info);

/**
 * @brief Reads the highest score from the score file.
 *
 * @return Highest score, or -1 on file read error.
 */
int readHighScore();

#endif  // SCORE_FILE_H