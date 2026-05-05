/*
  ==================================
  CLI rendering and input management
  ==================================
*/

#include "include/cli.h"

// ===========================================
// Internal helpers for drawing boxes and text

static void drawBox(int x, int y) {
  mvprintw(y, x, "+");
  for (int i = 0; i < CLI_FIELD_WIDTH; i++) printw("-");
  printw("+");

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    mvprintw(y + 1 + i, x, "|");
    mvprintw(y + 1 + i, x + CLI_FIELD_WIDTH + 1, "|");
  }

  mvprintw(y + FIELD_HEIGHT + 1, x, "+");
  for (int i = 0; i < CLI_FIELD_WIDTH; i++) printw("-");
  printw("+");
}

static void drawTextInBoxCentered(int x, int y, int row, const char *text) {
  int len = (int)strlen(text);
  int px = x + 1 + (CLI_FIELD_WIDTH - len) / 2;
  int py = y + 1 + row;
  mvprintw(py, px, "%s", text);
}

// ===================================
// Internal screen rendering functions

static void drawStartScreen(GameInfo_t *game_info) {
  int x = 2, y = 1;

  drawBox(x, y);

  drawTextInBoxCentered(x, y, 3, "TETRIS");
  drawTextInBoxCentered(x, y, 4, "by suzettem");
  drawTextInBoxCentered(x, y, 8, "Enter your name:");

  char name_line[MAX_NAME_LENGTH + 5];
  snprintf(name_line, sizeof(name_line), "[ %s ]", game_info->player);
  drawTextInBoxCentered(x, y, 9, name_line);

  drawTextInBoxCentered(x, y, 16, "ENTER to start");
  drawTextInBoxCentered(x, y, 17, "ESC to exit");
}

static void drawPauseScreen(GameInfo_t *game_info) {
  int x = 2, y = 1;

  drawBox(x, y);

  drawTextInBoxCentered(x, y, 3, "GAME PAUSED");

  drawTextInBoxCentered(x, y, 7, "Player:");
  char name_display[MAX_NAME_LENGTH];
  snprintf(name_display, sizeof(name_display), "%s", game_info->player);
  char *end = name_display + strlen(name_display) - 1;
  while (end >= name_display && *end == ' ') {
    *end = '\0';
    end--;
  }
  drawTextInBoxCentered(x, y, 8, name_display);

  drawTextInBoxCentered(x, y, 10, "Score:");
  char score_line[MAX_SCORE_LENGTH];
  snprintf(score_line, sizeof(score_line), "%d", game_info->score);
  drawTextInBoxCentered(x, y, 11, score_line);

  drawTextInBoxCentered(x, y, 16, "SPACE to continue");
  drawTextInBoxCentered(x, y, 17, "ESC to finish game");
}

static void drawGameOverScreen(GameInfo_t *game_info) {
  int x = 2, y = 1;

  drawBox(x, y);

  drawTextInBoxCentered(x, y, 3, "GAME OVER");

  drawTextInBoxCentered(x, y, 6, "Player:");

  char name_display[MAX_NAME_LENGTH];
  snprintf(name_display, sizeof(name_display), "%s", game_info->player);
  char *end = name_display + strlen(name_display) - 1;
  while (end >= name_display && *end == ' ') {
    *end = '\0';
    end--;
  }
  drawTextInBoxCentered(x, y, 7, name_display);

  drawTextInBoxCentered(x, y, 9, "Score:");
  char score_line[MAX_SCORE_LENGTH];
  snprintf(score_line, sizeof(score_line), "%d", game_info->score);
  drawTextInBoxCentered(x, y, 10, score_line);

  drawTextInBoxCentered(x, y, 12, "High score:");
  char high_score_line[MAX_SCORE_LENGTH];
  snprintf(high_score_line, sizeof(high_score_line), "%d",
           game_info->high_score);
  drawTextInBoxCentered(x, y, 13, high_score_line);

  drawTextInBoxCentered(x, y, 16, "ENTER to try again");
  drawTextInBoxCentered(x, y, 17, "ESC to exit");
}

static void drawField(GameInfo_t *game_info) {
  int offset_x = 2, offset_y = 1;

  drawBox(offset_x - 1, offset_y - 1);

  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      int cell = game_info->field[y][x];

      attron(COLOR_PAIR(cell));
      mvprintw(offset_y + y, offset_x + x * 2, "%s", cell ? PIXEL : VOID);
      attroff(COLOR_PAIR(cell));
    }
  }
}

static void drawSidebar(GameInfo_t *game_info) {
  int x = 25, y = 0;
  int x_offset = CLI_FIELD_WIDTH / 3;

  drawBox(x, y);

  drawTextInBoxCentered(x, y, 1, "Next:");

  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      int cell = game_info->next[i][j];
      attron(COLOR_PAIR(cell));
      mvprintw(y + 4 + i, x + x_offset + 1 + j * 2, "%s", cell ? PIXEL : VOID);
      attroff(COLOR_PAIR(cell));
    }
  }

  drawTextInBoxCentered(x, y, 8, "Score:");
  char score_line[MAX_SCORE_LENGTH];
  snprintf(score_line, sizeof(score_line), "%d", game_info->score);
  drawTextInBoxCentered(x, y, 9, score_line);

  drawTextInBoxCentered(x, y, 11, "High score:");
  char high_score_line[MAX_SCORE_LENGTH];
  snprintf(high_score_line, sizeof(high_score_line), "%d",
           game_info->high_score);
  drawTextInBoxCentered(x, y, 12, high_score_line);

  drawTextInBoxCentered(x, y, 14, "Level:");
  char level_line[3];
  snprintf(level_line, sizeof(level_line), "%d", game_info->level);
  drawTextInBoxCentered(x, y, 15, level_line);

  drawTextInBoxCentered(x, y, 17, "SPACE to pause");
  drawTextInBoxCentered(x, y, 18, "ESC to finish");
}

// ==================
// User input polling

UserInput_t getInput() {
  static int last_key = -2;  // No key has been pressed yet
  static int repeat_counter = 0;

  UserInput_t input;
  input.key = -1;  // No key pressed
  input.hold = 0;

  int k = getch();

  if (k == ERR) {
    // No input: reset hold tracking
    repeat_counter = 0;  // Prevent holding an empty input
    last_key = -1;
  } else {
    // Input received: process hold logic
    input.key = k;

    if (k == last_key) {
      repeat_counter++;
    } else {
      repeat_counter = 0;
      last_key = k;
    }

    input.hold = (repeat_counter >= HOLD_THRESHOLD);
  }

  return input;
}

// ===============================
// CLI initialization and shutdown

void cliInit() {
  initscr();  // Initialize ncurses screen
  cbreak();   // Disable line buffering
  noecho();   // Disable input echo
  keypad(stdscr,
         TRUE);           // Enable special keys support
  nodelay(stdscr, TRUE);  // Enable non-blocking input
  mousemask(0,
            NULL);               // Disable mouse events
  curs_set(0);                   // Hide cursor
  timeout(INPUT_POLL_INTERVAL);  // Input polling interval

  if (has_colors()) {
    start_color();
    use_default_colors();

    init_pair(1, COLOR_CYAN, -1);     // I
    init_pair(2, COLOR_BLUE, -1);     // J
    init_pair(3, COLOR_MAGENTA, -1);  // L, T
    init_pair(4, COLOR_YELLOW, -1);   // O
    init_pair(5, COLOR_GREEN, -1);    // S
    init_pair(6, COLOR_RED, -1);      // Z
  }
}

void cliEnd() { endwin(); }

// =========================
// Main rendering dispatcher

void drawingManager() {
  GameState_t state = getCurrentState();
  GameInfo_t game_info = getCurrentGameInfo();

  clear();

  switch (state) {
    case STATE_START:
      drawStartScreen(&game_info);
      break;
    case STATE_PAUSE:
      drawPauseScreen(&game_info);
      break;
    case STATE_GAME_OVER:
      drawGameOverScreen(&game_info);
      break;
    default:
      drawField(&game_info);
      drawSidebar(&game_info);
      break;
  }
  refresh();
}