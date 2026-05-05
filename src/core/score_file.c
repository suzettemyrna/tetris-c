/*
  =========================================
  High score file reading and writing logic
  =========================================
*/

#define _GNU_SOURCE

#include "include/score_file.h"

bool writeScoreIntoFile(GameInfo_t game_info) {
  bool res = FALSE;

  char player[MAX_NAME_LENGTH + 1];
  if (game_info.player) {
    strncpy(player, game_info.player, MAX_NAME_LENGTH);
  } else {
    player[0] = '\0';
  }
  player[MAX_NAME_LENGTH] = '\0';

  int score = game_info.score;

  FILE *f = fopen(SCORE_FILE, "a");
  if (f) {
    if (fprintf(f, "%-*s %d\n", MAX_NAME_LENGTH, player, score) > 0) {
      res = TRUE;
    }
    fclose(f);
  }

  return res;
}

int readHighScore() {
  int res = -1;  // error

  FILE *f = fopen(SCORE_FILE, "r");
  if (f) {
    int high_score = 0;
    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, f) != -1) {
      size_t L = strlen(line);

      if (L <= MAX_NAME_LENGTH) continue;
      if (line[MAX_NAME_LENGTH] != ' ') continue;

      // Points to the beginning of the score substring
      char *score_str = line + MAX_NAME_LENGTH + 1;

      // The number must start with a digit
      if (!isdigit((unsigned char)score_str[0])) continue;

      // Parse score manually to validate file contents
      int score = 0;
      int ok = 1;
      for (int i = 0; score_str[i] && ok; i++) {
        if (score_str[i] == '\n') break;
        if (!isdigit((unsigned char)score_str[i])) {
          ok = 0;
        }
        score = score * 10 + (score_str[i] - '0');
      }
      if (!ok) continue;

      if (score <= MAX_POINTS && score > high_score) high_score = score;
    }

    free(line);
    fclose(f);
    res = high_score;
  }

  return res;
}