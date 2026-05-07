#include "../src/core/game_info.c"
#include "include/test_helpers.h"

// =====
// TESTS

START_TEST(tc_init_and_clean) {
  mock_field_control_reset();
  mock_tetromino_reset();
  mock_score_file_reset();

  initGameInfo();

  GameInfo_t gi = getCurrentGameInfo();
  ck_assert_ptr_nonnull(gi.field);
  ck_assert_ptr_nonnull(gi.next);
  ck_assert_int_ge(gi.level, 1);
  ck_assert_int_ge(gi.speed, 1);

  cleanGameInfo();

  GameInfo_t gi2 = getCurrentGameInfo();
  ck_assert_int_eq(gi2.score, 0);
  ck_assert_int_eq(gi2.level, 0);
  ck_assert_ptr_null(gi2.field);
  ck_assert_ptr_null(gi2.next);
}
END_TEST

START_TEST(tc_resetGameInfo_behavior) {
  mock_field_control_reset();
  mock_tetromino_reset();
  mock_score_file_reset();

  mock_set_readHighScore(12345);

  initGameInfo();

  mock_set_current_field_value(1);

  game_info.score = 777;
  game_info.level = 5;

  resetGameInfo();

  GameInfo_t after = getCurrentGameInfo();

  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      ck_assert_int_eq(game_info.field[y][x], 0);
    }
  }

  for (int y = 0; y < TETROMINO_SIZE; ++y) {
    for (int x = 0; x < TETROMINO_SIZE; ++x) {
      ck_assert_int_eq(game_info.next[y][x] == 0, 1);
    }
  }

  ck_assert_int_eq(after.score, 0);
  ck_assert_int_eq(after.level, MIN_LEVEL);
  ck_assert_int_eq(after.high_score, 12345);

  cleanGameInfo();
}
END_TEST

START_TEST(tc_updateField_copies_current_field) {
  mock_field_control_reset();
  mock_tetromino_reset();
  mock_score_file_reset();

  initGameInfo();

  mock_set_current_field_value(7);

  updateField();

  for (int y = 0; y < FIELD_HEIGHT; ++y)
    for (int x = 0; x < FIELD_WIDTH; ++x)
      ck_assert_int_eq(game_info.field[y][x], 7);

  cleanGameInfo();
}
END_TEST

START_TEST(tc_newTetromino_and_updateNext) {
  mock_field_control_reset();
  mock_tetromino_reset();
  mock_score_file_reset();

  int piece[TETROMINO_SIZE][TETROMINO_SIZE] = {
      {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
  mock_set_random_piece(piece);

  ActiveTetromino_t active = {0};
  for (int y = 0; y < TETROMINO_SIZE; y++)
    for (int x = 0; x < TETROMINO_SIZE; x++)
      active.base.shape[y][x] = piece[y][x];
  active.x = 1;
  active.y = 2;
  mock_set_active_piece(active);

  initGameInfo();

  cleanGameInfo();
  initGameInfo();

  int spawn_before = mock_get_spawn_called();
  int upd_before = mock_get_updateFieldWithActive_called();

  newTetromino();
  ck_assert_int_ge(mock_get_spawn_called(), spawn_before + 1);
  ck_assert_int_ge(mock_get_updateFieldWithActive_called(), upd_before + 1);

  for (int y = 0; y < TETROMINO_SIZE; ++y)
    for (int x = 0; x < TETROMINO_SIZE; ++x)
      ck_assert_int_eq(game_info.next[y][x], piece[y][x]);

  cleanGameInfo();
}
END_TEST

START_TEST(tc_moveTetromino_calls_updateField) {
  mock_field_control_reset();
  mock_tetromino_reset();
  mock_score_file_reset();

  ActiveTetromino_t active = {0};
  active.x = 3;
  active.y = 4;
  mock_set_active_piece(active);

  initGameInfo();

  int before = mock_get_updateFieldWithActive_called();
  moveTetromino(ACTION_LEFT);
  ck_assert_int_ge(mock_get_updateFieldWithActive_called(), before + 1);

  cleanGameInfo();
}
END_TEST

START_TEST(tc_gameOver_and_merge) {
  mock_field_control_reset();
  mock_tetromino_reset();
  mock_score_file_reset();

  mock_set_GI_checkGameOver_return(1);
  mock_set_GI_checkMerge_return(1);

  initGameInfo();

  ck_assert(gameOverCondition());
  ck_assert(isThereMerge());

  cleanGameInfo();
}
END_TEST

START_TEST(tc_writeDownScore_behavior) {
  mock_field_control_reset();
  mock_tetromino_reset();
  mock_score_file_reset();

  initGameInfo();

  game_info.score = 0;
  writeDownScore();
  ck_assert_int_eq(mock_get_score_file_write_calls(), 0);

  game_info.score = 500;
  writeDownScore();
  ck_assert_int_eq(mock_get_score_file_write_calls(), 1);

  cleanGameInfo();
}
END_TEST

START_TEST(tc_updateGameInfoPlayerName_behavior) {
  mock_field_control_reset();
  mock_tetromino_reset();
  mock_score_file_reset();

  // initGameInfo() has not been called yet, so player == NULL
  updateGameInfoPlayerName("ABC");
  // Function must handle NULL state without crashing

  initGameInfo();

  strcpy(game_info.player, "OLDNAME");
  updateGameInfoPlayerName(NULL);
  ck_assert_str_eq(game_info.player, "OLDNAME");

  const char *newname = "TESTPLAYER";
  updateGameInfoPlayerName(newname);
  ck_assert_str_eq(game_info.player, newname);

  cleanGameInfo();
}
END_TEST

START_TEST(tc_updateScore_cases_1_2_3_4) {
  mock_field_control_reset();
  mock_tetromino_reset();
  mock_score_file_reset();

  initGameInfo();

  // case 1
  game_info.score = 0;
  game_info.high_score = 0;
  mock_set_clearCompletedLines_return(1);
  updateScore();
  ck_assert_int_eq(game_info.score, 100);
  ck_assert_int_eq(game_info.high_score, 100);

  // case 2
  game_info.score = 0;
  game_info.high_score = 0;
  mock_set_clearCompletedLines_return(2);
  updateScore();
  ck_assert_int_eq(game_info.score, 300);
  ck_assert_int_eq(game_info.high_score, 300);

  // case 3
  game_info.score = 0;
  game_info.high_score = 0;
  mock_set_clearCompletedLines_return(3);
  updateScore();
  ck_assert_int_eq(game_info.score, 700);
  ck_assert_int_eq(game_info.high_score, 700);

  // case 4
  game_info.score = 0;
  game_info.high_score = 0;
  mock_set_clearCompletedLines_return(4);
  updateScore();
  ck_assert_int_eq(game_info.score, 1500);
  ck_assert_int_eq(game_info.high_score, 1500);

  cleanGameInfo();
}
END_TEST

// =====
// SUITE

Suite *game_info_suite(void) {
  Suite *s = suite_create("game_info");
  TCase *tc_core = tcase_create("core");

  tcase_add_test(tc_core, tc_init_and_clean);
  tcase_add_test(tc_core, tc_resetGameInfo_behavior);
  tcase_add_test(tc_core, tc_updateField_copies_current_field);
  tcase_add_test(tc_core, tc_newTetromino_and_updateNext);
  tcase_add_test(tc_core, tc_moveTetromino_calls_updateField);
  tcase_add_test(tc_core, tc_gameOver_and_merge);
  tcase_add_test(tc_core, tc_writeDownScore_behavior);
  tcase_add_test(tc_core, tc_updateGameInfoPlayerName_behavior);
  tcase_add_test(tc_core, tc_updateScore_cases_1_2_3_4);

  suite_add_tcase(s, tc_core);
  return s;
}

// ====
// MAIN

int main(void) {
  int failed = 0;
  Suite *s = game_info_suite();
  SRunner *sr = srunner_create(s);

  printf("Running game_info tests...\n");
  srunner_run_all(sr, CK_NORMAL);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  if (failed == 0)
    printf("All game_info tests passed\n\n");
  else
    printf("Game_info tests failed: %d\n\n", failed);

  return failed;
}